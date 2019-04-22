//
//  Chop.cpp
//  Chop
//
//  Created by Michael Andrews on 8/9/17.
//  Copyright © 2017 Michael Andrews. All rights reserved.
//

#include "Chop.hpp"

monoBlocks chop::function(const mono& v) {
    
    size_t numberOfBlocks = 0;
    if (blockSize < v.size()) {
        numberOfBlocks = 1 + (v.size() - blockSize) / oneShift;
    }
    
    monoBlocks chopped_v(numberOfBlocks, blockSize);
    
    for (size_t i=0; i<numberOfBlocks; ++i) {
        size_t shift = i * oneShift;
        
        for (size_t j=0; j<blockSize; ++j) {
            chopped_v[i][j] = v[shift + j];
        }
    }
    return chopped_v;
}

mono unchop::function(const monoBlocks& v) {
    
    size_t numberOfBlocks = v.size();
    if (numberOfBlocks == 0) { return mono(0); }
    size_t blockSize = v[0].size();
    
    mono unchopped_v( blockSize + (numberOfBlocks - 1) * oneShift, 0);
    
    for (size_t i=0; i<numberOfBlocks; ++i) {
        size_t shift = i * oneShift;
        
        for (size_t j=0; j<blockSize; ++j) {
            unchopped_v[shift + j] += v[i][j];
        }
    }
    return unchopped_v;
}


mono granularStretch::function(const mono &v) {
    monoBlocks chopped = chop(oneShiftChop, window.size())(v);
    (multiply_by_window(window))(chopped);
    mono unchopped = unchop(oneShiftUnchop)(chopped);
    return unchopped;
}

void amplify::function(mono& v) {
    for (size_t n=0, N=v.size(); n<N; ++n) { v[n] *= amplification; }
}
