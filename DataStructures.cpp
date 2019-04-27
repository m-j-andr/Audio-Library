//
//  DataStructures.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "DataStructures.hpp"

monoBlocks::monoBlocks(size_t numberOfBlocks, size_t blockSize)   :
                       blocks(numberOfBlocks,   mono(blockSize,0)) {}

stereo::stereo(const mono& l, const mono& r)
       : l(l), r(r) { assert( l.size() == r.size() ); }

stereoBlocks::stereoBlocks(const monoBlocks& l, const monoBlocks& r)
       : l(l), r(r) { assert( l.size() == r.size() );
    
       if (l.size() > 0 || r.size() > 0) { assert( l[0].size() == r[0].size() ); }  }


mono        m2mForVectorize::operator() (const mono&       v) { return function(v); }
void   mInPlaceForVectorize::operator() (      mono&       v) {        function(v); }
monoBlocks m2mbForVectorize::operator() (const mono&       v) { return function(v); }
mono       mb2mForVectorize::operator() (const monoBlocks& v) { return function(v); }

stereo       m2mForVectorize::operator()  (const stereo&       v) {
    return stereo      (operator()(v.l), operator()(v.r));
}
stereoBlocks m2mForVectorize::operator()  (const stereoBlocks& v) {
    return stereoBlocks(operator()(v.l), operator()(v.r));
}
stereoBlocks m2mbForVectorize::operator() (const stereo&       v) {
    return stereoBlocks(operator()(v.l), operator()(v.r));
}
stereo       mb2mForVectorize::operator() (const stereoBlocks& v) {
    return stereo      (operator()(v.l), operator()(v.r));
}

void mInPlaceForVectorize::operator() (stereo&       v) {
    operator()(v.l); operator()(v.r);
}
void mInPlaceForVectorize::operator() (stereoBlocks& v) {
    operator()(v.l); operator()(v.r);
}


monoBlocks m2mForVectorize::operator() (monoBlocks v) {
    size_t i;
    const size_t N=v.size();
    
#ifdef _OPENMP
    #pragma omp parallel shared(v) private(i)
    #pragma omp for schedule(static)
    for (i=0; i<N; ++i) { v[i] = operator()(v[i]); }

#else
    for (i=0; i<N; ++i) { v[i] = operator()(v[i]); }

#endif
    return v;
}

void mInPlaceForVectorize::operator() (monoBlocks& v) {
    size_t i;
    const size_t N=v.size();

#ifdef _OPENMP
    #pragma omp parallel shared(v) private(i)
    #pragma omp for schedule(static)
    for (i=0; i<N; ++i) { operator()(v[i]); }

#else
    for (i=0; i<N; ++i) { operator()(v[i]); }
    
#endif
}
