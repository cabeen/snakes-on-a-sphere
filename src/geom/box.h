#ifndef BOX_H
#define BOX_H

#include "algebra.h"
#include "interval.h"

struct Box
{
    Box()
    {
        x = Interval();
        y = Interval();
        z = Interval();
    }

    Box(const Vector3 &a, const Vector3 &b){
        x = Interval(a.x, b.x);
        y = Interval(a.y, b.y);
        z = Interval(a.z, b.z);
    }

    Box(const Interval &x, const Interval &y, const Interval &z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Box join(const Box &box) const
    {
        return Box(x.join(box.x), y.join(box.y), z.join(box.z));
    }

    Box xfm(const Matrix4x4 &xfm) const
    {
        vector<Vector4> ps;

        ps.push_back(Vector4(x.min, y.min, z.min, 1));
        ps.push_back(Vector4(x.min, y.min, z.max, 1));
        ps.push_back(Vector4(x.min, y.max, z.min, 1));
        ps.push_back(Vector4(x.min, y.max, z.max, 1));

        ps.push_back(Vector4(x.max, y.min, z.min, 1));
        ps.push_back(Vector4(x.max, y.min, z.max, 1));
        ps.push_back(Vector4(x.max, y.max, z.min, 1));
        ps.push_back(Vector4(x.max, y.max, z.max, 1));

        Interval nx;
        Interval ny;
        Interval nz;

        for (int i = 0; i < ps.size(); i++) {
            Vector4 np = xfm * ps[i];
            if (i == 0) {
                nx.min = nx.max = np.x;
                ny.min = ny.max = np.y;
                nz.min = nz.max = np.z;
            } else {
                nx.min = fmin(nx.min, np.x);
                nx.max = fmax(nx.max, np.x);
                ny.min = fmin(ny.min, np.y);
                ny.max = fmax(ny.max, np.y);
                nz.min = fmin(nz.min, np.z);
                nz.max = fmax(nz.max, np.z);
            }
        }

        return Box(nx, ny, nz);
    }

    Box growFraction(double f) const
    {
        return Box(x.growFraction(f), y.growFraction(f), z.growFraction(f));
    }

    Interval x;
    Interval y;
    Interval z;
};

#endif // BOX_H
