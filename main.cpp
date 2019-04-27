//
//  main.cpp
//  Audio
//
//  Created by Michael Andrews on 4/20/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include <iostream>
#include "DataStructures.hpp"
#include "WAV.hpp"          // safer with -D _BOOST
#include "FM.hpp"
#include "FFT.hpp"
#include "Interpolate.hpp"
#include "Blocks.hpp"
#include "Chop.hpp"
#include "Windows.hpp"
using namespace std;

int main() {
    cout << "You can do audio related things!" << endl;
    
    mono m(2*44100);
    for (size_t t=0, T=m.size(); t<T; ++t) {
        m[t] = sin(1888.8 * t / 44100.0);
    }
    write("/Users/mjandr/Documents/Audio/sineWave.wav", m);
    
    return 0;
}
