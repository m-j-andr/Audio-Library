//
//  Interpolate.cpp
//  Interpolate
//
//  Created by Michael Andrews on 7/10/17.
//  Copyright © 2017 Michael Andrews. All rights reserved.
//

#include "Interpolate.hpp"

double interpolatedValue(const mono& sig, double index, size_t half_number_of_terms) {
    size_t rounded_index = static_cast<size_t>(index);
    size_t start = 0;
    size_t end   = sig.size();
    
    if (rounded_index > half_number_of_terms) {
        start = rounded_index - half_number_of_terms;
    }
    if (rounded_index + half_number_of_terms < sig.size()) {
        end = rounded_index + half_number_of_terms;
    }
    
    double value = 0;
    for (size_t i=start; i<end; ++i) {
        value += sig[i] * sinc(index - static_cast<double>(i));
    }
    return value;
}

mono interpolatedStretch::function(const mono &sig) {
    size_t newLength = static_cast<size_t>( static_cast<double>(sig.size()) * ratio );
    
    mono stretched(newLength);
    for (size_t t=0; t<newLength; ++t) {
        double s = static_cast<double>(t) / ratio;
        stretched[t] = interpolatedValue(sig, s);
    }
    return stretched;
}

double sinc(double x) {
    if (x == 0) { return 1; }
    else { return sin(acos(-1) * x) / (acos(-1) * x); }
}
