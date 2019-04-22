//
//  AAC.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef AAC_hpp
#define AAC_hpp

#include <iostream>
#include "DataStructures.hpp"
#include "Blocks.hpp"
#include "MDCT.hpp"

/*
 These functions implement the AAC transforms.
 
 To analyze, we:
    . chop up the signal into overlapping blocks;
    . multiply each of the blocks by a carefully chosen window function;
    . apply the MDCT to each block.

 To synthesize, we:
    . apply the IMDCT to each block;
    . multiply each of the blocks by the window function used for analysis;
    . sum up the overlapping blocks.
 
 We include a bool called "progress",
 in case we want status updates printed to the console.
*/

struct analysis : m2mbForVectorize {
    monoBlocks function(const mono& t);
    const mono& window;
    bool progress;
    
    analysis(const mono& window, bool progress = false)
                : window(window), progress(progress) {}
};

struct synthesis : mb2mForVectorize {
    mono function(const monoBlocks& f);
    const mono& window;
    bool progress;
    
    synthesis(const mono& window, bool progress = false)
                 : window(window), progress(progress) {}
};

#endif /* AAC_hpp */
