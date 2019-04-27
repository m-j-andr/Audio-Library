//
//  FM.cpp
//  FM
//
//  Created by Michael Andrews on 3/20/19.
//  Copyright © 2019 Michael Andrews. All rights reserved.
//

#include "FM.hpp"


const double FMfunction::dt = 1.0 / (4.0 * 44100.0);


FMfunction::FMfunction(const RtoRfunc func, double period,
                       dbl freq, dbl amp, dbl shift, dbl phase)
        : x(0), val(0),

          func(func), period(period),
          freq(freq), amp(amp), shift(shift), phase(phase)

        { update_val(); }


onceFM::onceFM(dbl cF, dbl mA, dbl mF)

    : aux  (mySine, 1,       mF, mA, cF),
      func (mySine, 1, &aux.val        ) {}


twiceFM::twiceFM(dbl ocF, dbl omA,
                 dbl  cF, dbl  mA, dbl mF)

    : aux2 (mySine, 1,        mF,  mA,  cF),
      aux1 (mySine, 1, &aux2.val, omA, ocF),
      func (mySine, 1, &aux1.val          ) {}


void FMfunction::update_x  () { x += freq * dt; while (x > period) { x -= period; }
                                                while (x <     0 ) { x += period; } }
void FMfunction::update_val() { val = shift + amp * func(x + phase); }
void FMfunction::update    () { update_x(); update_val(); }

void onceFM::update() {
    for (size_t t=0; t<4; ++t) {
        func.update();
         aux.update();
    }
}

void twiceFM::update() {
    for (size_t t=0; t<4; ++t) {
        func.update();
        aux1.update();
        aux2.update();
    }
}

double  onceFM::getVal() const { return func.val; }
double twiceFM::getVal() const { return func.val; }


double mySine(double x) { return sin( (8 * atan(1.0)) * x ); }
double mySquare(double x) {
    if (0   < x && x < 0.5) { return  1; }
    if (0.5 < x && x < 1.0) { return -1; }
    else                    { return  0; }
}
