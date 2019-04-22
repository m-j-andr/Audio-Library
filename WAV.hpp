//
//  WAV.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/5/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef WAV_hpp
#define WAV_hpp

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

/*
 We make functions to enable reading and writing WAV files.
 
 We can read and write mono and stereo.
 We can read 16-bit integers and 32-bit floats.
 We can write 32-bit floats.
 
 We do not bother writing with 16-bit integers.
 We do not bother reading or writing with 32-bit integers.
 We have not looked into how to handle reading or writing 24-bit integers.
 
 Our header struct handles JUNK correctly.
 It is lazy about footers, but does detect their presence.
 
 To deal with integer types of a fixed width we include <cstdint>.
 To deal with 32-bit floats we include <boost/cstdfloat.hpp>.
*/

#include <cstdint>
#include <boost/cstdfloat.hpp>
#include <fstream>
#include "DataStructures.hpp"

struct WAVheader {
    const uint8_t  riff_header[4] = {'R','I','F','F'};
          uint32_t bytes_after;
    const uint8_t  wave_header[4] = {'W','A','V','E'};
    
    bool           junk;
    const uint8_t  junk_header[4] = {'J','U','N','K'};
          uint32_t junk_length;
    
    const uint8_t  fmt_header[4]  = {'f','m','t',' '};
    const uint32_t fmt_length     = 16;
          uint16_t audio_fmt;
          uint16_t channels;
          uint32_t sample_rate_per_chan;
          uint32_t byt_per_sec;
          uint16_t block_size_in_bytes;
          uint16_t bit_depth_of_sample;
    
    const uint8_t  data_header[4] = {'d','a','t','a'};
          uint32_t data_length;
    
    bool           footer;
          size_t   samples_per_channel;
    
    WAVheader(size_t samples_per_channel, uint16_t channels,
              uint16_t audio_fmt, uint16_t bit_depth_of_sample,
              uint32_t sample_rate = 44100);
    
    WAVheader(const mono&   signal, uint32_t sample_rate = 44100);
    WAVheader(const stereo& signal, uint32_t sample_rate = 44100);
    
    WAVheader(std::string fileName);
    
private:
    void correct();
};

void write(std::string fileName, const mono&   signal, uint32_t sample_rate = 44100);
void write(std::string fileName, const stereo& signal, uint32_t sample_rate = 44100);

std::ostream& operator<< (std::ostream& out, const WAVheader& head);

mono   read_m(std::string fileName);
stereo read_s(std::string fileName);


template <class F, class T>
void write(std::ofstream& fout, const F& from, const T& to);
template <class F, class T>
void read(std::ifstream& fin, F& from, const T& to);

#endif /* WAV_hpp */
