#ifndef WHEEL_HPP
#define WHEEL_HPP

struct Wheel {
    dBodyID body;
    dGeomID geom;
    dReal radius;
    dReal thickness;
    dReal mass;
};

#endif