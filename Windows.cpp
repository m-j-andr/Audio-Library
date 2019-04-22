//
//  Windows.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "Windows.hpp"

std::vector<double> sine(size_t N) {
    std::vector<double> w(N);
    for (size_t n=0; n<N; ++n) {
        w[n] = sin(std::acos(-1) * (n + 0.5) / N);
    }
    return w;
}
std::vector<double> Hann(size_t N) {
    std::vector<double> w(N);
    for (size_t n=0; n<N; ++n) {
        w[n] = 0.5 - 0.5 * cos( 2 * std::acos(-1) * (n + 0.5) / N );
    }
    return w;
}
std::vector<double> derived_Hann(size_t N) {
    assert(N%2 == 0);
    return derive(Hann(N/2 + 1));
}

std::vector<double> derive(std::vector<double> window) {
    double integral = 0;
    for (double& val:window) { integral += val; }
    for (double& val:window) { val /= integral; }
    
    size_t N = window.size()-1;
    std::vector<double> derived_w(2*N);
    
    integral = 0;
    for (size_t n=0; n<N; ++n) {
        integral += window[n];
        derived_w[n] = sqrt(integral);
    }
    integral = 0;
    for (size_t n=0; n<N; ++n) {
        integral += window[N-n];
        derived_w[2*N-1-n] = sqrt(integral);
    }
    return derived_w;
}
