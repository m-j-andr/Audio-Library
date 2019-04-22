//
//  WAV.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/5/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "WAV.hpp"

// Writing...
WAVheader::WAVheader(size_t samples_per_channel, uint16_t channels,
                     uint16_t audio_fmt, uint16_t bit_depth_of_sample,
                     uint32_t sample_rate) :

    bytes_after(0),                             // corrected later
    junk(false), junk_length(0),
    audio_fmt(audio_fmt),
    channels(channels),
    sample_rate_per_chan(sample_rate),
    byt_per_sec(0),                             // corrected later
    block_size_in_bytes(0),                     // corrected later
    bit_depth_of_sample(bit_depth_of_sample),
    data_length(0),                             // corrected later
    footer(false),
    samples_per_channel(samples_per_channel)

    { correct(); }

void WAVheader::correct()
{
    block_size_in_bytes = channels * (bit_depth_of_sample / 8);
    byt_per_sec = static_cast<uint32_t>(block_size_in_bytes) * sample_rate_per_chan;
    data_length = static_cast<uint32_t>(samples_per_channel) *
                  static_cast<uint32_t>(block_size_in_bytes);
    bytes_after = 36 + data_length;
}

WAVheader::WAVheader(const mono& signal, uint32_t sample_rate) :

    bytes_after(0),                     // corrected later
    junk(false), junk_length(0),
    audio_fmt(3),
    channels(1),
    sample_rate_per_chan(sample_rate),
    byt_per_sec(0),                     // corrected later
    block_size_in_bytes(0),             // corrected later
    bit_depth_of_sample(32),
    data_length(0),                     // corrected later
    footer(false),
    samples_per_channel(signal.size())

    { correct(); }

WAVheader::WAVheader(const stereo& signal, uint32_t sample_rate) :

    bytes_after(0),                     // corrected later
    junk(false), junk_length(0),
    audio_fmt(3),
    channels(2),
    sample_rate_per_chan(sample_rate),
    byt_per_sec(0),                     // corrected later
    block_size_in_bytes(0),             // corrected later
    bit_depth_of_sample(32),
    data_length(0),                     // corrected later
    footer(false),
    samples_per_channel(signal.size())

    { correct(); }

template <class F, class T>
void write(std::ofstream& fout, const F& from, const T& to) {
    const char* pos = reinterpret_cast <const char*> (&from);
    const char* end = reinterpret_cast <const char*> (&to);
    for (; pos != end; ++pos) { fout.put(*pos); }
}

void write(std::string fileName, const mono& signal, uint32_t sample_rate) {
    std::ofstream fout(fileName);
    
    WAVheader header(signal, sample_rate);
    write(fout, header.riff_header, header.junk  );
    write(fout, header.fmt_header , header.footer);
    
    const size_t N = signal.size();
    std::vector<boost::float32_t> v; v.reserve(N+1);
    for (const double& val:signal) { v.push_back(val); } v.push_back(0);
    write(fout, v[0], v[N]);
    
    fout.close();
}

void write(std::string fileName, const stereo& signal, uint32_t sample_rate) {
    std::ofstream fout(fileName);
    
    WAVheader header(signal, sample_rate);
    write(fout, header.riff_header, header.junk  );
    write(fout, header.fmt_header , header.footer);
    
    const size_t N = signal.size();
    std::vector<boost::float32_t> v; v.reserve(2*N+1);
    for (size_t i=0; i<N; i++) {
        v.push_back(signal.l[i]);
        v.push_back(signal.r[i]);
    }
    v.push_back(0);
    write(fout, v[0], v[2*N]);
    
    fout.close();
}




// Reading...
std::ostream& operator<< (std::ostream& out, const WAVheader& h) {
    
    out << "RIFF header     : " << h.riff_header[0] << h.riff_header[1]
                                << h.riff_header[2] << h.riff_header[3] << '\n';
    out << "After this      : " << h.bytes_after    << " bytes"         << '\n';
    out << "WAVE header     : " << h.wave_header[0] << h.wave_header[1]
                                << h.wave_header[2] << h.wave_header[3] << '\n';
    
    if (h.junk) {
    out << "JUNK header     : " << h.junk_header[0] << h.junk_header[1]
                                << h.junk_header[2] << h.junk_header[3] << '\n';
    out << "JUNK length     : " << h.junk_length    << " bytes"         << '\n';
    }

    out << "Format header   : " << h.fmt_header              << '\n';
    out << "Format length   : " << h.fmt_length  << " bytes" << '\n';
    out << "Audio format    : ";
    
    if (h.audio_fmt == 1) { out << "ints"; }
    if (h.audio_fmt == 3) { out << "floats"; }
    
    out << '\n';
    
    out << "Channels        : " << h.channels                                  << '\n';
    out << "Sample rate     : " << h.sample_rate_per_chan << " Hz per channel" << '\n';
    out << "Depth of sample : " << h.bit_depth_of_sample  << " bits"           << '\n';
    out << "Block size      : " << h.block_size_in_bytes  << " bytes"          << '\n';
    out << "Bytes per sec   : " << h.byt_per_sec                               << '\n';
    
    out << "Data header     : " << h.data_header[0] << h.data_header[1]
                                << h.data_header[2] << h.data_header[3]   << '\n';
    out << "Data length     : " << h.data_length    << " bytes"           << '\n';
    
    const uint32_t samplesPerChan = h.data_length  /
                                          static_cast<uint32_t>(h.block_size_in_bytes);
    const uint32_t lengthInSecs   = samplesPerChan / h.sample_rate_per_chan;
    
    out << "Samples / chan  : " << samplesPerChan                << '\n';
    out << "Length          : " << lengthInSecs / 60 << " mins "
                                << lengthInSecs % 60 << " secs " << '\n';
    
    if (h.footer) { out << "Footer          : ???" << '\n'; }
    return out;
}

template <class F, class T>
void read(std::ifstream& fin, F& from, const T& to) {
          char* pos = reinterpret_cast <      char*> (&from);
    const char* end = reinterpret_cast <const char*> (&to);
    for (; pos != end; ++pos) { *pos = fin.get(); }
}

WAVheader::WAVheader(std::string fileName) {
    std::ifstream fin(fileName);
    
    fin.ignore(4);                          // RIFF
    read(fin, bytes_after, wave_header);
    fin.ignore(4);                          // WAVE
    
    char J = fin.get();
    char U = fin.get();
    char N = fin.get();
    char K = fin.get();
    
    if (J == 'J' && U == 'U' && N == 'N' && K == 'K') {
        junk = true;
        read(fin, junk_length, fmt_header);
        fin.ignore(junk_length);            // junk
        fin.ignore(4);                      // fmt
    }
    else {
        junk = false;
        junk_length = 0;
        // already moved past               // fmt
    }
    
    fin.ignore(4);                          // 16
    read(fin, audio_fmt, data_header);
    
    fin.ignore(4);                          // data
    read(fin, data_length, footer);
    fin.ignore(data_length);
    
    const size_t pos = fin.tellg();
    const size_t len = bytes_after + 8;
    
    if (pos == len) { footer = false; } else { footer = true; }
    samples_per_channel = static_cast<size_t>(data_length)    /
                          static_cast<size_t>(block_size_in_bytes);
    
    fin.close();
}

mono read_m(std::string fileName) {
    WAVheader  header(fileName);
    std::ifstream fin(fileName);
    
    size_t junk = 0;
    if (header.junk) { junk += header.junk_length + 8; }
    fin.ignore(junk + 44);
    
    mono forReturn(header.samples_per_channel);
        
    if (header.audio_fmt == 1 && header.bit_depth_of_sample == 16) {
        for (double& val:forReturn) {
            int16_t data;
            fin.read(reinterpret_cast <char*> (&data), sizeof(int16_t));
            val  = data;
            val /= pow(2,16);
        }
    }
    if (header.audio_fmt == 3 && header.bit_depth_of_sample == 32) {
        for (double& val:forReturn) {
            boost::float32_t data;
            fin.read(reinterpret_cast <char*> (&data), sizeof(boost::float32_t));
            val = data;
        }
    }
    fin.close();
    return forReturn;
}

stereo read_s(std::string fileName) {
    WAVheader  header(fileName);
    std::ifstream fin(fileName);
    
    size_t junk = 0;
    if (header.junk) { junk += header.junk_length + 8; }
    fin.ignore(junk + 44);
    
    const size_t N = header.samples_per_channel;
    mono l(N);
    mono r(N);
        
    if (header.audio_fmt == 1 && header.bit_depth_of_sample == 16) {
      for (size_t i=0; i<N; ++i) {
        int16_t data;
        fin.read(reinterpret_cast<char*>(&data), sizeof(int16_t)); l[i] = data;
        fin.read(reinterpret_cast<char*>(&data), sizeof(int16_t)); r[i] = data;
        l[i] /= pow(2,16);
        r[i] /= pow(2,16);
      }
    }
    if (header.audio_fmt == 3 && header.bit_depth_of_sample == 32) {
      for (size_t i=0; i<N; ++i) {
        boost::float32_t data;
        fin.read(reinterpret_cast<char*>(&data), sizeof(boost::float32_t)); l[i] = data;
        fin.read(reinterpret_cast<char*>(&data), sizeof(boost::float32_t)); r[i] = data;
      }
    }
    fin.close();
    return stereo(l,r);
}
