//
//  Blocks.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef Blocks_hpp
#define Blocks_hpp

#include "DataStructures.hpp"

/*
 overlap_chop chops a mono track into overlapping blocks.
 
 overlap_sum sum recovers a mono track
 to which overlap_chop has been applied.
 
 The vectorization of multiply_by_window to monoBlocks
 multiplies every block of a signal by a specified window function.
*/

struct overlap_chop : m2mbForVectorize {
    monoBlocks function(const mono& v);
    size_t halfBlockSize;
    
    overlap_chop(size_t halfBlockSize) : halfBlockSize(halfBlockSize) {}
};

struct overlap_sum : mb2mForVectorize {
    mono function(const monoBlocks& v);
};


struct multiply_by_window : mInPlaceForVectorize {
    void function(mono& v);
    const mono& window;
    
    multiply_by_window(const mono& window) : window(window) {}
};

#endif /* Blocks_hpp */
