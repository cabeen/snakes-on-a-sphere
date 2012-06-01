#ifndef INTERSECTSHAPE_H
#define INTERSECTSHAPE_H

#include <algebra.h>
#include "ray.h"
#include "interval.h"
#include "box.h"
#include "hit.h"
#include "interval.h"

const double RADIUS = 0.5;
const double R2 = RADIUS * RADIUS;
const double RC2 = 4.0;
const Interval UNIT_INT(-RADIUS, RADIUS);
const Box UNIT_BOX(UNIT_INT, UNIT_INT, UNIT_INT);

class IntersectShape
{
public:
    static bool intersectXRect(const Ray &ray, const double &x, const Interval &yrange, const Interval &zrange, double &t);
    static bool intersectYRect(const Ray &ray, const double &y, const Interval &xrange, const Interval &zrange, double &t);
    static bool intersectZRect(const Ray &ray, const double &z, const Interval &xrange, const Interval &yrange, double &t);
    static bool intersectDisk(const Ray &ray, const double &v, const double &r2, double &t);
    static bool intersectXPlane(const Ray &ray, const double &x, double &t);
    static bool intersectYPlane(const Ray &ray, const double &y, double &t);
    static bool intersectZPlane(const Ray &ray, const double &z, double &t);
    static bool intersectFiniteCone(const Ray &ray, double &t);
    static bool intersectFiniteCylinder(const Ray &ray, double &t);
    static bool nearestFront(const double &ta, const double &tb, double &t);
    static bool intersectSphere(const Ray &ray, Hit &hit);
    static bool intersectCylinder(const Ray &ray, Hit &hit);
    static bool intersectCone(const Ray &ray, Hit &hit);
    static bool intersectCube(const Ray &ray, Hit &hit);
    static bool intersect(const Ray &ray, const Box &box);
    static bool intersect(const Box &a, const Box &b);
    static bool intersect(const Interval &a, const Interval &b);
};

#endif // INTERSECTSHAPE_H
