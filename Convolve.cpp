//
//  Convolve.cpp
//  Convolve
//
//  Created by Michael Andrews on 5/19/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "Convolve.hpp"


mono convolve(mono sig1, mono sig2) {
    
    // Calculate size of output.
    size_t N = sig1.size() + sig2.size();
    if (N != 0) { --N; }
    
    // Calculate size of FFT.
    size_t powerOfTwo = 1;
    while (powerOfTwo < N) { powerOfTwo *= 2; }
    
    // Perform FFT.
    // Normalization should take place in the IFFT
    // in order for the convolution formula to work.
    sig1.resize(powerOfTwo);
    sig2.resize(powerOfTwo);
    
    std::vector<cmplx> f1 = IFFT(complexify(sig1)); // conj later.
    std::vector<cmplx> f2 = IFFT(complexify(sig2)); // conj later.

    // Do pointwise multiplication.
    for (size_t k=0; k<powerOfTwo; ++k) { f1[k] *= f2[k]; }
    conj(f1); // The conj we omitted above.
    
    // Take IFFT.
    f1 = IFFT(f1); norm(f1);
    
    // Calculate output.
    sig1.resize(N);
    for (size_t k=0; k<N; ++k) { sig1[k] = f1[k].real(); }
    
    return sig1;
}

mono convolve(const mono& input, const mono& impulseResponse,
              size_t start, size_t end) {
    
    // Sort out various indices.
    size_t inputEnd  = std::min(input.size(), end);
    size_t outputEnd = inputEnd + impulseResponse.size();
    
    assert(start <= inputEnd);
    if (outputEnd != 0) { --outputEnd; }
    
    // sig is the part of the input to convolve.
    mono sig(inputEnd - start);
    for (size_t t=start;    t<inputEnd;  ++t) { sig[t - start]  =  input[t]; }
    sig = convolve(sig, impulseResponse);
    
    // Make output signal.
    mono output = input;
    output.resize(std::max(input.size(), outputEnd));
    
    for (size_t t=start;    t<inputEnd;  ++t) { output[t]  = sig[t - start]; }
    for (size_t t=inputEnd; t<outputEnd; ++t) { output[t] += sig[t - start]; }
    
    return output;
}

mono applyDelay(const mono& input, const mono& impulseResponse,
                size_t start, size_t end) {
    
    // Sort out various indices.
    size_t inputEnd  = std::min(input.size(), end);
    size_t outputEnd = inputEnd + impulseResponse.size();
    
    assert(start <= inputEnd);
    if (outputEnd != 0) { --outputEnd; }
    
    // Make output signal.
    mono output(std::max(input.size(), outputEnd));
    
    for (size_t t=0;                        t<start; ++t) { output[t] = input[t]; }
    for (size_t t=inputEnd, N=input.size(); t<N;     ++t) { output[t] = input[t]; }
    
    for (size_t delay=0, N=impulseResponse.size(); delay<N; ++delay) {
        if (impulseResponse[delay] != 0) {
            for (size_t t=start; t<inputEnd; ++t) {
                output[t + delay] += input[t] * impulseResponse[delay];
            }
        }
    }
    return output;
}






stereo convolve(const stereo& input, const mono& impulseResponse) {

    return stereo(convolve(input.l, impulseResponse),
                  convolve(input.r, impulseResponse));
}
stereo convolve(const stereo& input, const mono& impulseResponse,
                size_t start, size_t end) {

    return stereo(convolve(input.l, impulseResponse, start, end),
                  convolve(input.r, impulseResponse, start, end));
}
stereo applyDelay(const stereo& input, const mono& impulseResponse,
                  size_t start, size_t end) {

    return stereo(applyDelay(input.l, impulseResponse, start, end),
                  applyDelay(input.r, impulseResponse, start, end));
}
