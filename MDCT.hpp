//
//  MDCT.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef MDCT_hpp
#define MDCT_hpp

#include "DataStructures.hpp"
#include "FFT.hpp"

/*
 We implement algorithms for the MDCT and IMDCT which
 make use of the IFFT together with pre- and post-"twiddling".
 
 We wish to apply the MDCT and IMDCT to many blocks in a chopped up signal.
 To this end, we define functors and make use of "DataStructures.hpp".
 
 One benefit is that the transforms are automatically vectorized.
 Another, is that we can store values which we will reuse for every block.
 
 The constructors for the functors calculate:
    . a lookup table for the bit-reversal permutation that the IFFT uses;
    . appropriate "twiddle values"; and
    . in both cases, N is intended to be the window size.
 
 To understand the algorithm, look it up a textbook on the subject, or ask me.
*/

struct MDCT : m2mForVectorize {
    mono function(const mono& t);
    
    MDCT(size_t N) : orbits(orbitsOfSize2(N)), twiddle1(roots_of_unity(N)) {
        
        const size_t halfN = N/2;
        twiddle2.reserve(halfN);
        
        size_t n = N+2;
        const size_t m = 2*n;
        const size_t M = 4*N;
        
        for (size_t k=0; k<halfN; ++k) {
            twiddle2.push_back(exp_pi_i_n_over_M(n,M));
            n += m;
        }
    }
private:
    std::vector<size_t> orbits;
    std::vector<cmplx> twiddle1, twiddle2;
};

struct IMDCT : m2mForVectorize {
    mono function(const mono& f);
    
    IMDCT(size_t N) : orbits(orbitsOfSize2(N)), roots(roots_of_unity(N/2)) {
        
        twiddle1.reserve(N);
        twiddle2.reserve(N);
        
        const size_t m = N/2 + 1;
        const size_t M = 2*N;
        
        for (size_t k=0; k<N; ++k) {
            twiddle1.push_back( exp_pi_i_n_over_M(  k*m,N) );
        }
        for (size_t n=0; n<N; ++n) {
            twiddle2.push_back( exp_pi_i_n_over_M(2*n+m,M) );
        }
    }
private:
    std::vector<size_t> orbits;
    std::vector<cmplx> roots, twiddle1, twiddle2;
};

#endif /* MDCT_hpp */
