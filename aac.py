#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 24 19:00:07 2018

@author: mjandr
"""

#%% MDCT
import numpy as np

def pre_mdct(x, n, axis):
    # COPIED AND MINORLY EDITED FROM FFTPACK

    if n is None:
        n = x.shape[axis]

    assert (n>1 and n%4 == 0)

    if n < x.shape[axis]:
        index = [slice(None)] * x.ndim
        index[axis] = slice(0, n)
        x = x[tuple(index)]

    if x.shape[axis] < n:
        s = list(x.shape)
        index = [slice(None)] * len(s)
        index[axis] = slice(0, s[axis])
        s[axis] = n
        z = np.zeros(s, x.dtype.char)
        z[tuple(index)] = x
        x = z

    return x

import scipy.fftpack as spfft

def mdct(x, n=None, axis=-1, overwrite_x=False):

    x = pre_mdct(x, n, axis)            # THIS IS JUST ME TRYING 
    if axis != -1:                      # TO MAKE MY CODE 
        x = np.swapaxes(x, axis, -1)    # COMPATIBLE WITH FFTPACK

    t = np.arange(x.shape[-1])
    t = np.exp( np.pi * t / (len(t) * 1.0j) )

    tx = t * x
    z = np.split( spfft.fft(tx), 2, axis=-1 )[0]

    t = np.arange(z.shape[-1])
    t = np.exp( np.pi * (len(t) + 1) * (t + 0.5) / (len(t) * 2.0j) )

    tz = t * z
    r = np.real(tz)

    if axis != -1:                      # AND AGAIN
        r = np.swapaxes(r, axis, -1)
    return r

def imdct(x, n=None, axis=-1, overwrite_x=False):

    x = pre_mdct(x, n, axis)
    if axis != -1:                      # AND AGAIN
        x = np.swapaxes(x, axis, -1)

    x = np.concatenate((x, -np.flip(x, axis=-1)), axis=-1)

    t = np.arange(x.shape[-1])
    t = np.exp( 1.0j * np.pi * (len(t)/2 + 1) * t / len(t) )

    tx = t * x
    z = spfft.ifft(tx)

    t = np.arange(z.shape[-1])
    t = np.exp( 1.0j * np.pi * (t + len(t)/4 + 0.5) / len(t) )

    tz = t * z
    r = np.real(2 * tz)

    if axis != -1:                      # AND AGAIN
        r = np.swapaxes(r, axis, -1)
    return r

#%% WINDOWS
def sine(n): # the sine window
    t = np.arange(n)
    return np.sin( np.pi * (t+0.5) / n);

def hann(n): # the Hann window
    t = np.arange(n)
    return 0.5 - 0.5 * np.cos( 2 * np.pi * (t+0.5) / n)

def derive(w): # to ensure good overlaps
    n = w / np.sum(w)

    d1 = np.array( [ np.sum(n[:i+1]) for i in xrange(len(n)-1) ] )
    d2 = np.array( [ np.sum(n[i+1:]) for i in xrange(len(n)-1) ] )

    return np.concatenate(( d1**0.5, d2**0.5 ))

def d_hann(n): # the window we'll use
    assert n%2 == 0
    return derive(hann(n/2 + 1))

#%% CHOPPING AND SPLICING
def chop(d,lw):
    hlw = lw/2

    z = np.zeros( d[:hlw].shape )
    d = np.concatenate((z,d))

    if (len(d)%lw != 0):
        z = np.zeros( d[:lw - len(d)%lw].shape )
        d = np.concatenate((d,z))

    b0 = np.split( d, len(d)/lw )

    z = np.zeros( d[:hlw].shape )
    d = np.concatenate((d,z))

    b1 = np.split( d[hlw:], len(d)/lw )

    return np.array(b0+b1)

def splice(b,hlw):
    assert (b.ndim == 2) or (b.ndim == 3)

    if b.ndim == 2:
        b = b.reshape(-1)
    else:
        b = b.reshape(-1,2)

    z = np.zeros( b[:hlw].shape )
    d = np.concatenate((b[:len(b)/2],z)) + np.concatenate((z,b[len(b)/2:]))

    return d[hlw:-hlw]

#%% AAC
def analysis(t,w):
    b = chop(t,len(w))

    if b.ndim == 3:
        b = np.swapaxes(b, 1, 2)

    b = w * b 
    b = mdct(b)
    return b

def synthesis(f,w):
    b = imdct(f)
    b = w * b

    if b.ndim == 3:
        b = np.swapaxes(b, 1, 2)

    t = splice(b,len(w)/2)
    return t

#%% USEFUL ADD-ON TO SYNTHESIS
import scipy.io.wavfile as spwav

def synthesis_and_int(f, w) :
    t = synthesis(f, w)
    t = 32767 * t / np.max(np.abs(t))
    t = t.astype(np.int16)
    return t

#%% IDENTITY - confirms correctness
def do_nothing(name, N = 2048):
    i = spwav.read(name+'.wav')
    t = i[1]

    w = d_hann(N)
    f = analysis(t, w)
    t = synthesis_and_int(f, w)

    spwav.write(name+'_nothing.wav', i[0], t)

#do_nothing('euclid_stripped_corrected')

#%% APPLY FUNCTION TO SPECTRUM
def apply_func_to_spec(name, func, N = 2048):
    i = spwav.read(name+'.wav')
    t = i[1]

    w = d_hann(N)
    f = analysis(t, w)

    f = func(f)

    t = synthesis_and_int(f, w)
    spwav.write(name+'_func.wav', i[0], t)

#%% AN EXAMPLE
def power(f, exp):
    return np.sign(f)*(np.abs(f)**exp)

#apply_func_to_spec('euclid_stripped_corrected', lambda f: power(f,0.6))

#%% TIME STRETCH
def stretch(name, num, denom, N = 2048):
    assert type(num) == type(denom) == int and num > 0 and denom > 0

    i = spwav.read(name+'.wav')
    t = i[1]

    n = 4 * denom
    while n < N:
        n = 2*n

    w = d_hann(n)
    f = analysis(t, w)

    w = d_hann((num * n) / denom)
    f = pre_mdct(f, (num * f.shape[-1]) / denom, -1)

    t = synthesis_and_int(f, w)
    spwav.write(name+'_stretch.wav', i[0], t)

#stretch('euclid_stripped_corrected', 6, 5)
#stretch('euclid_stripped_corrected', 6, 7)

#%% BANDPASS
def bandpass(name, low = 0.0, high = 1.0, N = 2048):
    assert(0.0 <= low and low <= high and high <= 1.0)

    i = spwav.read(name+'.wav')
    t = i[1]

    w = d_hann(N)
    f = analysis(t, w)

    m = np.max(np.abs(f), axis=-1, keepdims=True)
    cutoff_l = low  * m
    cutoff_h = high * m

    mask = (np.abs(f) < cutoff_l) | (np.abs(f) > cutoff_h)
    f[mask] = 0

    t = synthesis_and_int(f, w)
    spwav.write(name+'_bandpass.wav', i[0], t)

#bandpass('/Users/mjandr/Documents/Audio/fractal_skulls_gg_stripped_corrected', 
#         0.1, 0.4, 2**20)

#%% CYCLICALLY PERMUTING THE GREATEST n FREQUENCIES, ZEROING THE REST
def permute_max(f, n):
    freq = f.copy()
    args = np.empty(n, dtype = np.int)

    for i in xrange(n):
        args[i] = np.argmax(np.abs(freq))
        freq[args[i]] = 0

    f[args] = f[np.roll(args, n/2)]

    other_args = np.array(list( set(range(len(f)))-set(args) ), dtype = np.int)
    f[other_args] = 0

#%% 
def permute_freq(name, window_size, n, vers = ''):
    i = spwav.read(name+'.wav')
    t = i[1]
    l = len(t)

    w = d_hann(window_size)
    f = analysis(t, w)

    np.apply_along_axis(permute_max, -1, f, n)

    t = synthesis_and_int(f, w)
    t = t[0:l]
    spwav.write(name+'_permute' + vers + '.wav', i[0], t)

#permute_freq('blank_space_mus', 4 * 32768, 256)

#%% AMPLIFYING MAX VALUE AND OTHER VALUES BY 2 DIFFERENT VALUES
def change_max(f, max_amp, rest_amp):
    g = np.max(np.abs(f), axis=-1, keepdims=True)
    return np.where(np.abs(f)==g, max_amp * f, rest_amp * f)

#%% RUNNING MULTIPLE TIMES, SHIFTED
def multiple_change_max(name, window_size, n, 
                        max_amp = 1.0, rest_amp = 1.0, vers = ''):
    i = spwav.read(name+'.wav')
    t = 0.0 + i[1]

    w = d_hann(window_size)

    shift = max(1, window_size / (2 * n))
    l = len(t)

    for j in xrange(n):
        print j,

        f = analysis(t[shift * j:l], w)
        f = change_max(f, max_amp, rest_amp)

        s = synthesis(f, w)
        t[shift * j:l] = s[0:l - shift * j]

    t = 32767 * t / np.max(np.abs(t))
    t = t.astype(np.int16)
    spwav.write(name+'_multiple_change_max' + vers + '.wav', i[0], t)

#multiple_change_max('blank_space_vox', 4096,   8,  max_amp = 4.00)
#multiple_change_max('blank_space_mus', 4096,   8,  max_amp = 4.00)
#multiple_change_max('blank_space_mus',  128,   8,  max_amp = 0.00, vers = '1')
#multiple_change_max('blank_space_vox', 4096, 512,  max_amp = 1.04, vers = '2')
#multiple_change_max('blank_space_vox', 4096,   1, rest_amp = 0.00, vers = '3')