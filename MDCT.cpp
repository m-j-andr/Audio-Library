//
//  MDCT.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "MDCT.hpp"

mono MDCT::function(const mono& t) {
    const size_t N = t.size();
    // assert (twiddle1.size() == N);
    
    std::vector<cmplx> twiddled_t(N);
    for (size_t n=0; n<N; ++n) { twiddled_t[n] = twiddle1[n] * t[n]; }
    std::vector<cmplx> aux_f = FIFFT(twiddled_t, twiddle1, indices);

    mono f(N/2);    
    for (size_t k=0, M=f.size(); k<M; ++k) {
        aux_f[k] *= twiddle2[k];
        f[k] = aux_f[k].real();
    }
    return f;
}

mono IMDCT::function(const mono& f) {
    const size_t M = f.size();
    const size_t N = 2*M;
    // assert (twiddle1.size() == N);
    
    std::vector<cmplx> twiddled_f(N);
    for (size_t k=0; k<M; ++k) {
        twiddled_f[k] =   twiddle1[k] * f[    k];
    }
    for (size_t k=M; k<N; ++k) {
        twiddled_f[k] = - twiddle1[k] * f[N-1-k];
    }
    
    std::vector<cmplx> twiddled_t = FIFFT(twiddled_f, roots, indices);
    for (size_t n=0; n<N; ++n) {
        twiddled_t[n] *= twiddle2[n];
    }
    
    mono t(N);
    for (size_t n=0; n<N; ++n) { t[n] = twiddled_t[n].real() / M; }
    return t;
}
