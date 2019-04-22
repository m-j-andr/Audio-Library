//
//  Blocks.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "Blocks.hpp"

monoBlocks overlap_chop::function(const mono& v) {
    
    const size_t len = v.size();
    const size_t blockSize = 2 * halfBlockSize;
    const size_t numberOfBlocks = len / halfBlockSize + 2;
    
    monoBlocks chopped_v(numberOfBlocks, blockSize);
    
    for(size_t n=halfBlockSize; n<blockSize; ++n) {
        chopped_v[0][n] = v[n-halfBlockSize];
    }
    
    size_t i = 1;
    size_t shift = 0;
    
    for(; shift + blockSize <= len; ++i, shift += halfBlockSize) {
        for (size_t n=0; n<blockSize; ++n) {
            chopped_v[i][n] = v[ shift + n ];
        }
    }
    
    size_t n = 0;
    for(; shift + n < len; ++n) { chopped_v[i][n] = v[ shift + n ]; }
    
    ++i; shift += halfBlockSize;
    
    n = 0;
    for(; shift + n < len; ++n) { chopped_v[i][n] = v[ shift + n ]; }
    
    return chopped_v;
}

mono overlap_sum::function(const monoBlocks& v) {
    
    size_t numberOfBlocks = v.size();
    size_t blockSize = 0;
    if (numberOfBlocks > 0) { blockSize = v[0].size(); }
    
    mono summed_v( (numberOfBlocks + 1) * (blockSize / 2), 0);
    
    for(size_t i=0; i<numberOfBlocks; ++i) {
        size_t shift = i * (blockSize / 2);
        
        for (size_t n=0; n<blockSize; ++n) {
            summed_v[ shift + n ] += v[i][n];
        }
    }
    return summed_v;
}


void multiply_by_window::function(mono& v) {
    // assert (v.size() == window.size());
    for (size_t n=0, N=v.size(); n<N; ++n) { v[n] *= window[n]; }
}
