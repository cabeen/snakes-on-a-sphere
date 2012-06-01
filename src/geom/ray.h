#ifndef RAY_H
#define RAY_H

#include <vector.h>

struct Ray
{
    Ray(const Vector4 p, const Vector4 d)
    {
        this->s = -1;
        this->p = p;
        this->d = d;
        this->p.w = 1.0;
        this->d.w = 0.0;
    }

    Ray(int s, const Vector4 p, const Vector4 d)
    {
       this->s = s;
       this->p = p;
       this->d = d;
       this->p.w = 1.0;
       this->d.w = 0.0;
    }

    Vector4 shoot(double t) const
    {
        return Vector4(p.x + t * d.x, p.y + t * d.y, p.z + t * d.z, 1);
    }

    Ray xfm(const Matrix4x4 &xfm) const
    {
        return Ray(s, xfm * p, xfm * d);
    }

    int s;
    Vector4 p;
    Vector4 d;
};

#endif // RAY_H
