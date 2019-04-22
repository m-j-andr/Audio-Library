//
//  FFT.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef FFT_hpp
#define FFT_hpp

#include <complex>
#include <vector>
#include <cmath>
#include <utility>

/*
 We give our own implementation of the FFT.
 
 Mathematically, I prefer the normalization 1/N to happen in the FFT
 rather than the IFFT. I also prefer to focus on defining the IFFT.
 This is because I feel that the formula for the IFFT more clearly
 demonstrates the purpose of the FFT and IFFT:
 the IFFT formula expresses a signal as a sum of waves of given amplitudes.
 
 We define our FFTs in terms of the IFFT.
 We give a slower and more readable implementation of the IFFT.
 We also give a faster implementation of the IFFT (FIFFT).
 The speed gains are made by the fact that we avoid
 recursion and expensive copying. The algorithm is almost in-place.
 
 . complexify is self-explanatory.
 . conj conjugates every element in a vector.
 . norm divides a vector by its size.
 
 . exp_pi_n_over_M is self-explanatory.
 . roots_of_unity(M) generates M equally spaced roots of unity
   on the upper half of the unit circle: 1, exp(pi i / M), ...
 
 . reverseBits(k,bits) takes the least significant "bits" bits in k
   and returns a "bits" bit number with them reversed.
   e.g. reverseBits(3,4) == 12 since 0011_2 becomes 1100_2.
 . reverseBits(N) expects N to be a power of 2.
   It then returns a vector of the numbers 0,1,2,...,N-1
   with their bits reversed.
 
 . The reason we have overloaded IFFTs which take in roots_of_unity
   is to avoid recomputing these values when performing many IFFTs.
 . Similarly, "indices" will be reverseBits(f.size()) and
   we will avoid recomputing this vector when performing many IFFTs.
*/

typedef std::complex<double> cmplx;
std::vector<cmplx> complexify(const std::vector<double>& t);

// Simple operations on complex vectors
void conj(std::vector<cmplx>& f);
void norm(std::vector<cmplx>& f);

// Roots of unity
cmplx exp_pi_i_n_over_M(size_t n, size_t M);
std::vector<cmplx> roots_of_unity(size_t M);

// Reversing bits - useful for the faster IFFT
size_t reverseBits(size_t k, size_t bits);
std::vector<size_t> reverseBits(size_t N);

// FFT defined in terms of IFFT
std::vector<cmplx> FFT(const std::vector<double>& t);
std::vector<cmplx> FFT(const std::vector<cmplx>& t);

// Readable IFFT - slow
std::vector<cmplx> IFFT(const std::vector<cmplx>& f);
std::vector<cmplx> IFFT(const std::vector<cmplx>& f,
                        const std::vector<cmplx>& roots_of_unity);

// Less readable IFFT - much faster
std::vector<cmplx> FIFFT(const std::vector<cmplx>& f,
                         const std::vector<cmplx>& roots_of_unity);
std::vector<cmplx> FIFFT(const std::vector<cmplx>& f,
                         const std::vector<cmplx>& roots_of_unity,
                         const std::vector<size_t>& indices);

#endif /* FFT_hpp */
