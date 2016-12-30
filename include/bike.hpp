#ifndef BIKE_HPP
#define BIKE_HPP

#include "wheel.hpp"
#include <math.h>

struct Bike {
    dBodyID body;
    dGeomID geom;
    dReal radius;
    dReal thickness;
    dReal mass;

    dReal wheelbase; // w
    dReal trail; // c
    dReal steerAxisTilt; // λ
    dReal gravity; // g
    dReal speed; // v

    dReal rR, mR;
    dReal rF, mF;

    Wheel rear;
    Wheel front;

    // Values taken from Meijaard et al.
    Bike() {
        wheelbase = 1.02;
        trail = 0.08;
        steerAxisTilt = M_PI / 10.0;
        gravity = 9.81;

        rR = 0.3;
        mR = 2;
        rF = 0.3; // chosen to be same as rear wheel
        mF = 2;
    }
};

#endif