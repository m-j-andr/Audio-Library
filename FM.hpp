//
//  FM.hpp
//  FM
//
//  Created by Michael Andrews on 3/20/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#ifndef FM_hpp
#define FM_hpp

#include <cmath>

/*
 The FMfunction class allows us to read through the values of "func",
 a function with period "period" at a frequency of "freq".
 This frequency is allowed to be a pointer to a changing value,
 so that we can perform FM synthesis.
 
 "amp", "shift", and "phase" are used as in the formula
             shift + amp * func(x + phase)
 These values can also be modulated.
 
 onceFM and doubleFM allow FM synthesis with specified parameters:
             modulation frequency        (mF)
             modulation amplitude        (mA)
             center frequency            (cF)
             outer modulation amplitude (omA)
             outer center frequency     (ocF)
*/

class dbl {
public:
    dbl(const double* ptr) : p(ptr) {}
    dbl(const double  val) : d(val) {}
    
    operator double() const { return *p; }
    
    dbl(const dbl& toCopy) : d(toCopy.d) {
        if (toCopy.p != &toCopy.d) {
            p = toCopy.p;
        }
    }
    
private:
    const double* p = &d;
    const double  d =  0;
};

typedef double (*RtoRfunc)(double);


class FMfunction {
public:
    FMfunction(const RtoRfunc func, double period,
               dbl freq,
               dbl amp   = 1.0,
               dbl shift = 0.0,
               dbl phase = 0.0);
    double val;
    void update();

private:
    double x;
    void update_x  ();
    void update_val();

    const RtoRfunc func;
    const double period;
    dbl freq, amp, shift, phase;
    
    static const double dt;
};

class onceFM {
public:
    onceFM(dbl cF, dbl mA, dbl mF);

    double getVal() const;
    void update();
    
private:
    // Order is important for initializer to work as intended
    FMfunction aux;
    FMfunction func;
};

class twiceFM {
public:
    twiceFM(dbl ocF, dbl omA,
            dbl  cF, dbl  mA, dbl mF);

    double getVal() const;
    void update();
    
private:
    // Order is important for initializer to work as intended
    FMfunction aux2;
    FMfunction aux1;
    FMfunction func;
};


double mySine(double x);
#endif /* FM_hpp */
