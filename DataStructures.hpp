//
//  DataStructures.hpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef DataStructures_hpp
#define DataStructures_hpp

#include <cassert>
#include <vector>

#ifdef _OPENMP
#include <omp.h> // multithreading on blocks
#endif

/*
 The first data structures are suited for storing:
    . a mono track;
    . a mono track that has been chopped into blocks;
    . a stereo track;
    . a stereo track that has been chopped into blocks.
 
 The m2mForVectorize struct allows us to define a
 mono-to-mono function, and for it to be extended
 correctly to:
    . a stereo-to-stereo function;
    . a monoBlocks-to-monoBlocks function;
    . a stereoBlocks-to-stereoBlocks function.
 
 The remaining data structures allows similar vectorizations.
 
 The cost of these automized extensions is using functors,
 but, for the MDCT, functors will actually allow better runtime.
*/

typedef std::vector<double> mono;

struct monoBlocks {
    monoBlocks(size_t numberOfBlocks, size_t blockSize);
    
          mono& operator[] (size_t i)       { return blocks[i]; }
    const mono& operator[] (size_t i) const { return blocks[i]; }
    
    size_t size() const { return blocks.size(); }
    
private:
    std::vector<mono> blocks;
};

struct stereo {
    mono l;
    mono r;
    
    stereo(const mono& l, const mono& r);
    
    size_t size() const { assert(l.size() == r.size()); return l.size(); }
};

struct stereoBlocks {
    monoBlocks l;
    monoBlocks r;
    
    stereoBlocks(const monoBlocks& l, const monoBlocks& r);
};


struct m2mForVectorize {
    virtual mono function(const mono& v) = 0;
    
    mono         operator() (const mono&         v);
    monoBlocks   operator() (      monoBlocks    v);
    stereo       operator() (const stereo&       v);
    stereoBlocks operator() (const stereoBlocks& v);
};

struct mInPlaceForVectorize {
    virtual void function(mono& v) = 0;
    
    void operator() (mono&         v);
    void operator() (monoBlocks&   v);
    void operator() (stereo&       v);
    void operator() (stereoBlocks& v);
};

struct m2mbForVectorize {
    virtual monoBlocks function(const mono& v) = 0;
    
    monoBlocks   operator() (const mono&   v);
    stereoBlocks operator() (const stereo& v);
};

struct mb2mForVectorize {
    virtual mono function(const monoBlocks& v) = 0;
    
    mono   operator() (const monoBlocks&   v);
    stereo operator() (const stereoBlocks& v);
};

#endif /* DataStructures_hpp */
