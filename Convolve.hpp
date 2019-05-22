//
//  Convolve.hpp
//  Convolve
//
//  Created by Michael Andrews on 5/19/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef Convolve_hpp
#define Convolve_hpp

#include "DataStructures.hpp"
#include "FFT.hpp"
#include <cassert>
#include <algorithm>

/*
 convolve(sig1, sig2) calculates the convolution of two signals.
        output.size() = sig1.size() + sig2.size() - 1
 
 convolve(input, impulseResponse, start, end)
 convolves input[start:end] with impulseResponse,
 while leaving the rest of the input in tact.
 
 applyDelay(input, impulseResponse, start, end)
 has the same output (up to numerical errors) as
 convolve(input, impulseResponse, start, end),
 but the implementation is different.
 
 convolve makes use of the FFT and is suitable for
 impulse responses with many non-zero terms.
 applyDelay does the convolution directly
 where required and is suitable for
 impulse responses with only a few non-zero terms.
*/
 
mono convolve(mono sig1, mono sig2);
mono convolve(const mono& input, const mono& impulseResponse,
              size_t start, size_t end = -1);

mono applyDelay(const mono& input, const mono& impulseResponse,
                size_t start = 0, size_t end = -1);

// Extend to stereo.
stereo convolve(const stereo& input, const mono& impulseResponse);
stereo convolve(const stereo& input, const mono& impulseResponse,
                size_t start, size_t end = -1);

stereo applyDelay(const stereo& input, const mono& impulseResponse,
                size_t start = 0, size_t end = -1);

#endif /* Convolve_hpp */
