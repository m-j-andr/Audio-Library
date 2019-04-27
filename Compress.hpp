//
//  Compress.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef Compress_hpp
#define Compress_hpp

#include "DataStructures.hpp"
#include "LosePrecision.hpp"
#include <algorithm>        // std::max

/*
 All of these functions are intended to be applied to
 a signal analyzed with the "analysis" function in "AAC.hpp".
*/

struct loseBits : mInPlaceForVectorize {
    void function(mono& f);
    size_t bitsToLose;
    
    loseBits(size_t bitsToLose) : bitsToLose(bitsToLose) {}
};

struct compressRel : mInPlaceForVectorize {
    void function(mono& f);
    double thresholdLower;
    double thresholdHigher;
    
    compressRel(double thresholdLower, double thresholdHigher = 1.0)
      : thresholdLower(thresholdLower),  thresholdHigher(thresholdHigher) {
        assert( 0.0 <= thresholdLower &&
                       thresholdLower <= thresholdHigher &&
                                         thresholdHigher <= 1.0 );
    }
};

struct compress : mInPlaceForVectorize {
    void function(mono& f);
    size_t discard;
    size_t bitsToLose;
    
    compress(size_t discard, size_t bitsToLose)
          : discard(discard), bitsToLose(bitsToLose) {}
};

std::vector<size_t> ind_by_mag(const std::vector<double>& f);

#endif /* Compress_hpp */
