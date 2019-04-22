//
//  AAC.cpp
//  AAC Compression - Rework
//
//  Created by Michael Andrews on 4/4/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "AAC.hpp"

monoBlocks analysis::function(const mono& t) {
    if (progress) { std::cout << "starting analysis"    << std::endl; }
    
    monoBlocks blocks = overlap_chop(window.size()/2)(t);
    if (progress) { std::cout << "finished chopping"    << std::endl; }
    
    (multiply_by_window(window))(blocks);
    if (progress) { std::cout << "finished multiplying" << std::endl; }
    
    monoBlocks f = MDCT(window.size())(blocks);
    if (progress) { std::cout << "finished MDCT"        << std::endl; }
    
    if (progress) { std::cout << "finished analysis"    << std::endl; }
    return f;
}

mono synthesis::function(const monoBlocks& f) {
    if (progress) { std::cout << "starting synthesis"   << std::endl; }
    
    monoBlocks blocks = IMDCT(window.size())(f);
    if (progress) { std::cout << "finished IMDCT"       << std::endl; }
    
    (multiply_by_window(window))(blocks);
    if (progress) { std::cout << "finished multiplying" << std::endl; }
    
    mono t = overlap_sum()(blocks);
    if (progress) { std::cout << "finished summing"     << std::endl; }
    
    if (progress) { std::cout << "finished synthesis"   << std::endl; }
    return t;
}
