#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#include "wheel.hpp"
#include "bike.hpp"

static dWorldID world;
static dSpaceID space;
static dGeomID ground;
static dJointGroupID contactGroup;

dsFunctions fn;

Bike bike;

void createWheel(Wheel &w, dReal radius, dReal mass,
        dReal thickness, dReal x, dReal y, dReal z) {
    dMass m;
    w.radius = radius;
    w.mass = mass;
    w.thickness = thickness;

    w.body = dBodyCreate(world);
    dMassSetZero(&m);
    dMassSetCylinder(&m, w.mass, 2, w.radius, w.thickness);
    dBodySetMass(w.body, &m);
    dBodySetPosition(w.body, x, y, z);

    dMatrix3 R;
    dRFromAxisAndAngle(R,1,0,0,M_PI/2.0);
    dBodySetRotation(w.body, R);

    w.geom = dCreateCylinder(space, w.radius, w.thickness);
    dGeomSetBody(w.geom, w.body);
}

void createWheels(Bike &bike) {
    dReal thickness = 0.05;

    // Values taken from Meijaard et. al
    createWheel(bike.rear, bike.rR, bike.mR,
        thickness, 0, 0, bike.rR);
    createWheel(bike.front, bike.rF, bike.mF,
        thickness, bike.wheelbase, 0, bike.rF);
}

void createBike(Bike &bike) {
    createWheels(bike);
}

static void nearCallback(void *data, dGeomID o1, dGeomID o2) {
    const int N = 10;
    dContact contact[N];
    int isGround = ((ground == o1) || (ground == o2));
    int n = dCollide(o1, o2, N, &contact[0].geom, sizeof(dContact));
    if (isGround) {
        for (int i = 0; i < n; i++) {
            contact[i].surface.mode = dContactBounce;
            contact[i].surface.mu = dInfinity;
            contact[i].surface.bounce = 1.0;
            contact[i].surface.bounce_vel = 0.01;
            dJointID c = dJointCreateContact(world, contactGroup, &contact[i]);
            dJointAttach(c, dGeomGetBody(contact[i].geom.g1),
                            dGeomGetBody(contact[i].geom.g2));
        }
    }
}

void start() {
    static float xyz[3] = {0,-3,1};
    static float hpr[3] = {90,0,0};
    dsSetViewpoint(xyz, hpr);
}

static void simLoop(int pause) {
    const dReal *pos1, *pos2, *rot1, *rot2;
    dSpaceCollide(space, 0, &nearCallback);
    dWorldStep(world, 0.01);
    dJointGroupEmpty(contactGroup);

    // draw rear wheel
    dsSetColor(1,0,0);
    pos1 = dBodyGetPosition(bike.rear.body);
    rot1 = dBodyGetRotation(bike.rear.body);
    dsDrawCylinder(pos1, rot1, bike.rear.thickness, bike.rear.radius);

    // draw front wheel
    dsSetColor(0,0,1);
    pos2 = dBodyGetPosition(bike.front.body);
    rot2 = dBodyGetRotation(bike.front.body);
    dsDrawCylinder(pos2, rot2, bike.front.thickness, bike.front.radius);
}

void prepDrawStuff() {
    fn.version = DS_VERSION;
    fn.start = &start;
    fn.step = &simLoop;
    fn.command = NULL;
    fn.stop = NULL;
    fn.path_to_textures = "textures";
}

int main(int argc, char *argv[]) {
    prepDrawStuff();

    dInitODE();
    world = dWorldCreate();
    space = dHashSpaceCreate(0);
    contactGroup = dJointGroupCreate(0);

    dWorldSetGravity(world,0,0,-0.5);
    ground = dCreatePlane(space,0,0,1,0);

    createBike(bike);

    dsSimulationLoop(argc, argv, 800, 600, &fn);
    dWorldDestroy(world);
    dCloseODE();
    return 0;
}