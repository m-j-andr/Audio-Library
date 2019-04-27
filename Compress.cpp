//
//  Compress.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "Compress.hpp"

void loseBits::function(mono& f) {
    for(size_t k=0, N=f.size(); k<N; ++k) {
        loseSignificantBits(bitsToLose, f[k]);
    }
}

void compressRel::function(mono& f) {
    double maxAbs = 0;
    for(size_t k=0, N=f.size(); k<N; ++k) {
        maxAbs = std::max(maxAbs, abs(f[k]));
    }
    double cutoffLower  = thresholdLower  * maxAbs;
    double cutoffHigher = thresholdHigher * maxAbs;
    
    for(size_t k=0, N=f.size(); k<N; ++k) {
        if (abs(f[k]) < cutoffLower || cutoffHigher < abs(f[k])) { f[k] = 0; }
    }
}

void compress::function(mono& f) {
    std::vector<size_t> ind = ind_by_mag(f);
    
    for(size_t i=0; i<discard; ++i) {
        size_t k = ind[i];
        f[k] = 0;
    }
    for(size_t i=discard, N=f.size(); i<N; ++i) {
        size_t k = ind[i];
        loseSignificantBits(bitsToLose, f[k]);
    }
}

// a sorting algorithm
#include <map>

std::vector<size_t> ind_by_mag(const std::vector<double>& f) {
    size_t N=f.size();
    
    std::vector<double> v(N);
    for (size_t k=0; k<N; ++k) { v[k] = std::abs(f[k]); }
    
    std::map<double,std::vector<size_t>> index;
    for (size_t k=0; k<N; ++k) { index[ v[k] ].push_back(k); }
    
    sort(v.begin(),v.end());
    std::vector<size_t> ind_by_mag(N);
    
    for (size_t i=0, j=0; i<N; ++i) {
        ind_by_mag[i] = index[ v[i] ][j];
        if (++j == index[ v[i] ].size()) { j = 0; }
    }
    return ind_by_mag;
}
