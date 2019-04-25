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
    
    std::vector<cmplx> aux(N);
    for (size_t n=0; n<N; ++n) { aux[n] = twiddle1[n] * t[n]; }
    
    inPlaceIFFT(aux, twiddle1, orbits);

    mono f(N/2);    
    for (size_t k=0, M=f.size(); k<M; ++k) {
        aux[k] *= twiddle2[k];
        f[k] = aux[k].real();
    }
    return f;
}

mono IMDCT::function(const mono& f) {
    const size_t M = f.size();
    const size_t N = 2*M;
    // assert (twiddle1.size() == N);
    
    std::vector<cmplx> aux(N);
    for (size_t k=0; k<M; ++k) { aux[k] =   twiddle1[k] * f[    k]; }
    for (size_t k=M; k<N; ++k) { aux[k] = - twiddle1[k] * f[N-1-k]; }
    
    inPlaceIFFT(aux, roots, orbits);
    for (size_t n=0; n<N; ++n) { aux[n] *= twiddle2[n]; }
    
    mono t(N);
    for (size_t n=0; n<N; ++n) { t[n] = aux[n].real() / M; }
    return t;
}
