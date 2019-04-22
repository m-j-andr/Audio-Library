//
//  Windows.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef Windows_hpp
#define Windows_hpp

#include <vector>
#include <cmath>

/*
 The derive function makes sure that "the window overlap condition" is met.
 We'll normally use the derived Hann window.
*/

std::vector<double> derive(std::vector<double> window);

std::vector<double> sine(size_t N);
std::vector<double> Hann(size_t N);
std::vector<double> derived_Hann(size_t N);

#endif /* Windows_hpp */
