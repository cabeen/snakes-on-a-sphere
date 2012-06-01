#ifndef STATE_H
#define STATE_H

#include "algebra.h"
#include <vector>

struct Pose
{
    Vector4 pos;
    Vector4 dir;
};

struct State
{
    double barrierrad;
    double foodrad;
    double jointrad;
    double worldrad;

    double prevtime;
    double torque;
    double speed;

    int eaten;
    Vector4 eye;
    Vector4 look;
    Vector4 up;
    vector<Vector4> barrier;
    vector<Vector4> food;
    vector<Pose> path;
    vector<Pose> joints;

    void camera(int dx, int dy, int dz);
    void steer(bool left, bool right);
    bool update(float time);
    void init(int level);
};

Vector4 randomSphere(double r);

#endif // STATE_H
