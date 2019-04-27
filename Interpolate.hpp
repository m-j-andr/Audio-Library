//
//  Interpolate.hpp
//  Interpolate
//
//  Created by Michael Andrews on 7/10/17.
//  Copyright © 2017 Michael Andrews. All rights reserved.
//

#ifndef interpolate_hpp
#define interpolate_hpp

#include "DataStructures.hpp"
#include <cmath>    // sin, acos

/*
 interpolatedValue returns
 the correct interpolated value of a sampled signal
 assuming that there were no frequencies outside the Nyquist range
 in the original pre-sampled signal.
 
 With a constant sample rate,
 interpolatedStretch(1.0/100.0) is likely to create significant aliasing.
*/

double interpolatedValue(const mono& sig, double index,
                         size_t half_number_of_terms = 8);

struct interpolatedStretch : m2mForVectorize {
    mono function(const mono& sig);
    double ratio;
    
    interpolatedStretch (double ratio) : ratio(ratio) {}
};

double sinc(double x);

#endif /* interpolate_hpp */
