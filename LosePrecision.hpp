//
//  LosePrecision.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef LosePrecision_hpp
#define LosePrecision_hpp

#include <iostream>
#include <vector>
#include <cmath>

/*
 reconstructDouble demonstrates how doubles are stored internally.
 
 loseSignificantBits(n,d) sets n bits of d to 0.
 It starts with the least significant bit of the fractional part.
 After setting all of the fractional part to 0, it moves onto the exponent.
 It is not allowed to access the sign bit.
*/

void loseSignificantBits(size_t n, double& d);
double reconstructDouble(double d, bool details = false);

#endif /* LosePrecision_hpp */
