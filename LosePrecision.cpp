//
//  LosePrecision.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "LosePrecision.hpp"

void loseSignificantBits(size_t n, double& d) {
    assert (0 <= n && n <= 63);
    unsigned char* p = reinterpret_cast <unsigned char*> (&d);
    
    for (size_t i=0; 8*(i+1)<=n; ++i) { *(p+i) = 0; }
    
    size_t i =  n/8;
    *(p+i) >>= (n-8*i);
    *(p+i) <<= (n-8*i);
}

double reconstructDouble(double d, bool details) {
    const unsigned char* p = reinterpret_cast <unsigned char*> (&d);
    const std::vector<unsigned int> bytes = { *(p+0), *(p+1), *(p+2), *(p+3),
                                              *(p+4), *(p+5), *(p+6), *(p+7) };
    
    const unsigned int byte6_1stHalf = bytes[6] % (1 << 4);
    const unsigned int byte6_2ndHalf = bytes[6] / (1 << 4);
    
    const unsigned int byte7_minusLastBit = bytes[7] % (1 << 7);
    const unsigned int byte7_lastBit      = bytes[7] / (1 << 7);
    
    double forReturn;
    
    int exponent = byte6_2ndHalf + (1 << 4) * byte7_minusLastBit;
    
    if (exponent != 0) { forReturn = 1; }
    else               { forReturn = 0; exponent = 1;}
    
    exponent -= 1023;
    
    double aux = (1 << 4);
    forReturn += byte6_1stHalf / aux;
    
    for (size_t i=5; i!=static_cast<size_t>(-1); --i) {
        aux *= (1 << 8);
        forReturn += bytes[i] / aux;
    }
    
    forReturn *= pow(2,exponent);
    if (byte7_lastBit == 1) { forReturn *= -1; }
    
    if (details) {
        for (size_t i=0; i<8; ++i) {
            std::cout << "byte " << i << " is " << bytes[i] << std::endl;
        }
        std::cout << "byte6_1stHalf      is " << byte6_1stHalf      << std::endl;
        std::cout << "byte6_2ndHalf      is " << byte6_2ndHalf      << std::endl;
        std::cout << "byte7_minusLastBit is " << byte7_minusLastBit << std::endl;
        std::cout << "exponent           is " << exponent           << std::endl;
        std::cout << "signBit            is " << byte7_lastBit      << std::endl;
    }
    return forReturn;
}
