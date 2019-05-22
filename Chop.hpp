//
//  Chop.hpp
//  Chop
//
//  Created by Michael Andrews on 8/9/17.
//  Copyright © 2017 Michael Andrews. All rights reserved.
//

#include "DataStructures.hpp"
#include "Blocks.hpp"

/*
 These functors are more general than the ones in Block.hpp
 (which were made for AAC compression)
 in that they allow different overlaps.
 However, they're less careful about the ends of a signal.
*/
 
struct chop : m2mbForVectorize {
    monoBlocks function(const mono& v);
    size_t oneShift;
    size_t blockSize;
    
    chop(size_t oneShift, size_t blockSize) :
       oneShift(oneShift), blockSize(blockSize) {}
};

struct unchop : mb2mForVectorize {
    mono function(const monoBlocks& v);
    
    size_t oneShift;
    unchop(size_t oneShift) : oneShift(oneShift) {}
};


struct granularStretch : m2mForVectorize {
    mono function(const mono& v);
    size_t oneShiftChop;
    size_t oneShiftUnchop;
    const mono& window;
    
    granularStretch (size_t oneShiftChop,
                     size_t oneShiftUnchop,
                     const mono& window) : oneShiftChop(oneShiftChop),
                                           oneShiftUnchop(oneShiftUnchop),
                                           window(window) {}
};
