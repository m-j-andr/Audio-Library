//
//  FFT.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "FFT.hpp"

std::vector<cmplx> FFT(const std::vector<double>& t) {
    std::vector<cmplx> f = IFFT(complexify(t));
    conj(f); norm(f);
    return f;
}
std::vector<cmplx> FFT(const std::vector<cmplx>& t) {
    std::vector<cmplx> f = IFFT(t);
    for (size_t k=1, M=f.size(); k<M/2; ++k) {
        std::swap(f[k], f[M-k]);
    }
    norm(f);
    return f;
}
std::vector<cmplx> IFFT(const std::vector<cmplx>& f) {
    std::vector<cmplx> roots = roots_of_unity(f.size()/2);
    return FIFFT(f, roots);
}
std::vector<cmplx> IFFT(const std::vector<cmplx>& f,
                        const std::vector<cmplx>& roots_of_unity) {
    
    if (f.size() == 1) { return f; }
    
    assert (f.size() % 2 == 0);
    
    const size_t N = f.size();
    const size_t M = f.size() / 2;
    
    std::vector<cmplx> f_even(M);
    std::vector<cmplx> f_odd (M);
    
    for (size_t k=0; k<M; ++k) {
        f_even[k] = f[2*k  ];
        f_odd [k] = f[2*k+1];
    }
    
    const std::vector<cmplx> t_even = IFFT(f_even, roots_of_unity);
    const std::vector<cmplx> t_odd  = IFFT(f_odd , roots_of_unity);
    
    std::vector<cmplx> t(N);
    const size_t depth = roots_of_unity.size() / M;
    
    for (size_t n=0; n<M; ++n) {
        cmplx twiddle = roots_of_unity[depth * n];
        
        t[n  ] = t_even[n] + twiddle * t_odd[n];
        t[n+M] = t_even[n] - twiddle * t_odd[n];
    }
    return t;
}


std::vector<cmplx> FIFFT(const std::vector<cmplx>& f,
                         const std::vector<cmplx>& r) {
    
    const size_t N = f.size();
    const std::vector<size_t> i = reverseBits(N);
    return FIFFT(f, r, i);
}
std::vector<cmplx> FIFFT(const std::vector<cmplx>& f,
                         const std::vector<cmplx>& r,
                         const std::vector<size_t>& i) {
    
    // Make a copy of f with the indices permuted via bit-reversal.
    const size_t N = f.size();
    std::vector<cmplx> t; t.reserve(N);
    for (size_t k=0; k<N; ++k) { t.push_back( f[i[k]] ); }
    
    // Perform the IFFT non-recursively and almost in-place.
    for (size_t m=2, d=r.size(); m<=N; m*=2, d/=2) {
        for (size_t k=0; k<N; k+=m) {
            for (size_t j=0, M=m/2; j<M; ++j) {
                cmplx a = t[k  +j];
                cmplx b = t[k+M+j] * r[d*j];
                
                t[k  +j] = a+b;
                t[k+M+j] = a-b;
            }
        }
    }
    return t;
}

size_t reverseBits(size_t k, size_t bits) {
    size_t rev = 0;
    for (size_t i=0; i<bits; ++i) {
        rev += ((k>>i) & 1) << (bits-1-i);
    }
    return rev;
}
std::vector<size_t> reverseBits(size_t N) {
    const size_t bits = log2(N);
    assert (pow(2,bits) == N);
    
    std::vector<size_t> rev; rev.reserve(N);
    for (size_t k=0; k<N; ++k) { rev.push_back( reverseBits(k,bits) ); }
    return rev;
}


// Easy functions
std::vector<cmplx> complexify(const std::vector<double>& t) {
    std::vector<cmplx> c; c.reserve(t.size());
    for (double d : t) { c.push_back(d); }
    return c;
}
void conj(std::vector<cmplx>& f) {
    for (cmplx& z : f) { z = conj(z); }
}
void norm(std::vector<cmplx>& f) {
    size_t N = f.size();
    for (cmplx& z : f) { z /= N; }
}
cmplx exp_pi_i_n_over_M(size_t n, size_t M) {
    double pi = std::acos(-1);
    cmplx  i(0,1);
    double n_over_M = static_cast<double>(n) / M;
    return exp(pi * i * n_over_M);
}
std::vector<cmplx> roots_of_unity(size_t M) {
    std::vector<cmplx> roots; roots.reserve(M);
    
    for (size_t n=0; n<M; ++n) {
        roots.push_back(exp_pi_i_n_over_M(n,M));
    }
    return roots;
}
