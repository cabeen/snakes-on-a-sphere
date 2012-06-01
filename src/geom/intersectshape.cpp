#include "intersectshape.h"

bool IntersectShape::intersectXRect(const Ray &ray, const double &x, const Interval &yrange, const Interval &zrange, double &t)
{
    if (IntersectShape::intersectXPlane(ray, x, t))
        return yrange.contains(ray.p.y + t * ray.d.y) &&
               zrange.contains(ray.p.z + t * ray.d.z);
    else
        return false;
}

bool IntersectShape::intersectYRect(const Ray &ray, const double &y, const Interval &xrange, const Interval &zrange, double &t)
{
    if (IntersectShape::intersectYPlane(ray, y, t))
        return xrange.contains(ray.p.x + t * ray.d.x) &&
               zrange.contains(ray.p.z + t * ray.d.z);
    else
        return false;
}

bool IntersectShape::intersectZRect(const Ray &ray, const double &z, const Interval &xrange, const Interval &yrange, double &t)
{
    if (IntersectShape::intersectZPlane(ray, z, t))
        return yrange.contains(ray.p.y + t * ray.d.y) &&
               xrange.contains(ray.p.x + t * ray.d.x);
    else
        return false;
}

bool IntersectShape::intersectDisk(const Ray &ray, const double &y, const double &r2, double &t)
{
    if (IntersectShape::intersectYPlane(ray, y, t)) {
        double x = ray.p.x + t * ray.d.x;
        double z = ray.p.z + t * ray.d.z;

        return (x * x + z * z < r2);
    } else {
        return false;
    }
}

bool IntersectShape::intersectXPlane(const Ray &ray, const double &x, double &t)
{
    if (ray.d.x == 0)
        return false;

    t = (x - ray.p.x) / ray.d.x;

    return t > 0;
}

bool IntersectShape::intersectYPlane(const Ray &ray, const double &y, double &t)
{
    if (ray.d.y == 0)
        return false;

    t = (y - ray.p.y) / ray.d.y;

    return t > 0;
}

bool IntersectShape::intersectZPlane(const Ray &ray, const double &z, double &t)
{
    if (ray.d.z == 0)
        return false;

    t = (z - ray.p.z) / ray.d.z;

    return t > 0;
}

bool IntersectShape::nearestFront(const double &ta, const double &tb, double &t)
{
    if (ta <= 0 && tb <= 0)
        return false;

    if (ta <= 0)
        t = tb;
    else if (tb <= 0)
        t = ta;
    else
        t = fmin(ta, tb);

    return true;
}

bool IntersectShape::intersectFiniteCone(const Ray &ray, double &t)
{
    double a = RC2 * (ray.d.x * ray.d.x + ray.d.z * ray.d.z) - ray.d.y * ray.d.y;
    double b = 2 * (RC2 *(ray.p.x * ray.d.x + ray.p.z * ray.d.z) - ray.p.y * ray.d.y + RADIUS * ray.d.y);
    double c = RC2 * (ray.p.x * ray.p.x + ray.p.z * ray.p.z) - ray.p.y * ray.p.y + 2 * ray.p.y * RADIUS - R2;
    double det = b * b - 4 * a * c;

    if (det < 0)
        return false;

    double sqrtd = sqrt(det);
    double twoa = 2 * a;
    double ta = (-b - sqrtd) / twoa;
    double tb = (-b + sqrtd) / twoa;

    double ya = ray.p.y + ta * ray.d.y;
    double yb = ray.p.y + tb * ray.d.y;

    bool av = ya >= -RADIUS && ya <= RADIUS;
    bool bv = yb >= -RADIUS && yb <= RADIUS;

    if (av && bv) {
        return nearestFront(ta, tb, t);
    } else if (av) {
        t = ta;
        return true;
    } else if (bv) {
        t = tb;
        return true;
    } else  {
        return false;
    }
}

bool IntersectShape::intersectFiniteCylinder(const Ray &ray, double &t)
{
    double a = ray.d.x * ray.d.x + ray.d.z * ray.d.z;
    double b = 2 * (ray.p.x * ray.d.x + ray.p.z * ray.d.z);
    double c = ray.p.x * ray.p.x + ray.p.z * ray.p.z - R2 ;
    double det = b * b - 4 * a * c;

    if (det < 0)
        return false;

    double sqrtd = sqrt(det);
    double twoa = 2 * a;
    double ta = (-b - sqrtd) / twoa;
    double tb = (-b + sqrtd) / twoa;

    double ya = ray.p.y + ta * ray.d.y;
    double yb = ray.p.y + tb * ray.d.y;

    bool av = ya >= -RADIUS && ya <= RADIUS;
    bool bv = yb >= -RADIUS && yb <= RADIUS;

    if (av && bv) {
        return nearestFront(ta, tb, t);
    } else if (av) {
        t = ta;
        return true;
    } else if (bv) {
        t = tb;
        return true;
    } else  {
        return false;
    }
}

bool IntersectShape::intersectSphere(const Ray &ray, Hit &hit)
{
    double a = ray.d.x * ray.d.x + ray.d.z * ray.d.z + ray.d.y * ray.d.y;
    double b = 2.0 * (ray.p.x * ray.d.x + ray.p.z * ray.d.z + ray.p.y * ray.d.y);
    double c = ray.p.x * ray.p.x + ray.p.z * ray.p.z + ray.p.y * ray.p.y - R2 ;
    double det = b * b - 4 * a * c;

    if (det < 0)
        return false;

    double sqrtd = sqrt(det);
    double twoa = 2 * a;
    double ta = (-b - sqrtd) / twoa;
    double tb = (-b + sqrtd) / twoa;

    if (!nearestFront(ta, tb, hit.t))
        return false;

    Vector4 i = ray.shoot(hit.t);
    hit.n.x = i.x;
    hit.n.y = i.y;
    hit.n.z = i.z;
    hit.n.normalize();
    double ang = (i.z == i.x) ? 0.0 : atan2(i.z, i.x);
    hit.u = (ang < 0) ? -ang / (2 * M_PI) : 1.0 - ang / (2 * M_PI);
    hit.v = 1.0 - (asin(i.y / RADIUS) / M_PI + RADIUS);

    return true;
}

bool IntersectShape::intersectCylinder(const Ray &ray, Hit &hit)
{
    double t;
    double tc;
    bool ci = IntersectShape::intersectFiniteCylinder(ray, tc);

    double tt, tb;
    bool ti = IntersectShape::intersectDisk(ray, RADIUS, R2, tt);
    bool bi = IntersectShape::intersectDisk(ray, -RADIUS, R2, tb);

    if (ci && ti && !bi) {
        if (!nearestFront(tc, tt, t))
            return false;
    } else if (ci && !ti && bi) {
        if (!nearestFront(tc, tb, t))
            return false;
    } else if (!ci && ti && bi) {
        if (!nearestFront(tt, tb, t))
            return false;
    } else if (ci && !ti && !bi) {
        t = tc;
        if (t <= 0)
            return false;
    } else if (!ci && !ti && bi) {
        t = tb;
        if (t <= 0)
            return false;
    } else if (!ci && ti && !bi) {
        t = tt;
        if (t <= 0)
            return false;
    } else if (!ci && !ti && !bi) {
        return false;
    }

    hit.t = t;
    Vector4 i = ray.shoot(t);
    if (t == tc) {
        hit.n.x = 2 * i.x;
        hit.n.y = 0.0;
        hit.n.z = 2 * i.z;
        hit.n.normalize();
        double ang = (i.z == i.x) ? 0.0 : atan2(i.z, i.x);
        hit.u = (ang < 0) ? -ang / (2 * M_PI) : 1.0 - ang / (2 * M_PI);
        hit.v = 1.0 - i.y - RADIUS;
    } else {
        hit.n.x = 0;
        hit.n.y = (t == tt) ? 1.0 : -1.0;
        hit.n.z = 0;
        hit.u = i.x + RADIUS;
        hit.v = (t == tt) ? i.z + RADIUS : 1.0 - i.z - RADIUS;
    }

    return true;
}

bool IntersectShape::intersectCone(const Ray &ray, Hit &hit)
{
    double t;
    double tc;
    bool ci = IntersectShape::intersectFiniteCone(ray, tc);

    double tb;
    bool bi = IntersectShape::intersectDisk(ray, -RADIUS, R2, tb);

    if (ci && bi) {
        if (!nearestFront(tc, tb, t))
            return false;
    } else if (ci && !bi) {
        t = tc;
        if (t <= 0)
            return false;
    } else if (!ci && bi) {
        t = tb;
        if (t <= 0)
            return false;
    } else if (!ci && !bi) {
        return false;
    }


    hit.t = t;
    Vector4 i = ray.shoot(t);
    if (t == tc) {
        hit.n.x = 2.0 * RC2 * i.x;
        hit.n.y = -2.0 * (i.y - RADIUS);
        hit.n.z = 2.0 * RC2 * i.z;
        hit.n.normalize();
        double ang = (i.z == i.x) ? 0.0 : atan2(i.z, i.x);
        hit.u = (ang < 0) ? -ang / (2 * M_PI) : 1.0 - ang / (2 * M_PI);
        hit.v = 1.0 - i.y - RADIUS;
    } else {
        hit.n.x = 0;
        hit.n.y = -1;
        hit.n.z = 0;
        hit.u = i.x + RADIUS;
        hit.v = 1.0 - i.z - RADIUS;
    }

    return true;
}

bool IntersectShape::intersectCube(const Ray &ray, Hit &hit)
{
    bool init = true;
    double tf;
    if (IntersectShape::intersectXRect(ray, -RADIUS, UNIT_INT, UNIT_INT, tf))
        if (init || tf < hit.t) {
            hit.t = tf;
            init = false;
            Vector4 i = ray.shoot(hit.t);
            hit.u = i.z + RADIUS;
            hit.v = 1.0 - i.y - RADIUS;
            hit.n.x = -1;
            hit.n.y = 0;
            hit.n.z = 0;
        }

    if (IntersectShape::intersectXRect(ray, RADIUS, UNIT_INT, UNIT_INT, tf))
        if (init || tf < hit.t) {
            hit.t = tf;
            init = false;
            Vector4 i = ray.shoot(hit.t);
            hit.u = 1.0 - i.z - RADIUS;
            hit.v = 1.0 - i.y - RADIUS;
            hit.n.x = 1;
            hit.n.y = 0;
            hit.n.z = 0;
        }

    if (IntersectShape::intersectYRect(ray, -RADIUS, UNIT_INT, UNIT_INT, tf))
        if (init || tf < hit.t) {
            hit.t = tf;
            init = false;
            Vector4 i = ray.shoot(hit.t);
            hit.u = i.x + RADIUS;
            hit.v = 1.0 - i.z - RADIUS;
            hit.n.x = 0;
            hit.n.y = -1;
            hit.n.z = 0;
        }

    if (IntersectShape::intersectYRect(ray, RADIUS, UNIT_INT, UNIT_INT, tf))
        if (init || tf < hit.t) {
            hit.t = tf;
            init = false;
            Vector4 i = ray.shoot(hit.t);
            hit.u = i.x + RADIUS;
            hit.v = i.z + RADIUS;
            hit.n.x = 0;
            hit.n.y = 1;
            hit.n.z = 0;
        }

    if (IntersectShape::intersectZRect(ray, -RADIUS, UNIT_INT, UNIT_INT, tf))
        if (init || tf < hit.t) {
            hit.t = tf;
            init = false;
            Vector4 i = ray.shoot(hit.t);
            hit.u = 1.0 - i.x - RADIUS;
            hit.v = 1.0 - i.y - RADIUS;
            hit.n.x = 0;
            hit.n.y = 0;
            hit.n.z = -1;
        }

    if (IntersectShape::intersectZRect(ray, RADIUS, UNIT_INT, UNIT_INT, tf))
        if (init || tf < hit.t) {
            hit.t = tf;
            init = false;
            Vector4 i = ray.shoot(hit.t);
            hit.u = i.x + RADIUS;
            hit.v = 1.0 - i.y - RADIUS;
            hit.n.x = 0;
            hit.n.y = 0;
            hit.n.z = 1;
        }


    return !init;
}

bool IntersectShape::intersect(const Ray &ray, const Box &box)
{
    double t;
    return IntersectShape::intersectXRect(ray, box.x.min, box.y, box.z, t) ||
           IntersectShape::intersectXRect(ray, box.x.max, box.y, box.z, t) ||
           IntersectShape::intersectYRect(ray, box.y.min, box.x, box.z, t) ||
           IntersectShape::intersectYRect(ray, box.y.max, box.x, box.z, t) ||
           IntersectShape::intersectZRect(ray, box.z.min, box.x, box.y, t) ||
           IntersectShape::intersectZRect(ray, box.z.max, box.x, box.y, t);
}

bool IntersectShape::intersect(const Box &a, const Box &b)
{
    if (!intersect(a.x, b.x))
        return false;
    if (!intersect(a.y, b.y))
        return false;
    if (!intersect(a.z, b.z))
        return false;

    return true;
}

bool IntersectShape::intersect(const Interval &a, const Interval &b)
{
    return !(b.min > a.max || b.max < a.min);
}
