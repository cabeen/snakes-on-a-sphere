#include <matrix.h>
#include <iostream>
#define PI 3.1415926535897931
#define DEG2RAD 0.017453292519943295

// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v)
{
    Matrix4x4 out = Matrix4x4(
                        v.x, 0,   0,   0,
                        0,   v.y, 0,   0,
                        0,   0,   v.z, 0,
                        0,   0,   0,   1);

    return out;
}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v)
{
    Matrix4x4 out = Matrix4x4(
                        1, 0, 0, v.x,
                        0, 1, 0, v.y,
                        0, 0, 1, v.z,
                        0, 0, 0, 1);

    return out;
}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians)
{
    double cosr = cos(radians);
    double sinr = sin(radians);

    Matrix4x4 out = Matrix4x4(
                        1, 0,    0,     0,
                        0, cosr, -sinr, 0,
                        0, sinr, cosr,  0,
                        0, 0,    0,     1);

    return out;
}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians)
{
    double cosr = cos(radians);
    double sinr = sin(radians);

    Matrix4x4 out = Matrix4x4(
                        cosr,  0, sinr, 0,
                        0,     1, 0,    0,
                        -sinr, 0, cosr, 0,
                        0,     0, 0,    1);

    return out;
}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians)
{
    double cosr = cos(radians);
    double sinr = sin(radians);

    Matrix4x4 out = Matrix4x4(
                        cosr, -sinr, 0, 0,
                        sinr, cosr,  0, 0,
                        0,    0,     1, 0,
                        0,    0,     0, 1);

    return out;
}

Matrix4x4 getRotMat(const Vector4 &v, const REAL a)
{
    REAL sina = sin(a);
    REAL ocos = 1 - cos(a);

    REAL scale = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    Vector4 w = v / scale;
    Matrix4x4 wp = Matrix4x4(
                        0, -w.z, w.y, 0,
                        w.z, 0, -w.x, 0,
                        -w.y, w.x, 0, 0,
                        0,   0,   0,   0);
    Matrix4x4 wp2 = wp * wp;
    Matrix4x4 R = Matrix4x4::identity() + sina * wp + ocos * wp2;

    return R;
}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat(const Vector4 &p, const Vector4 &v, const REAL a)
{
    REAL sina = sin(a);
    REAL ocos = 1 - cos(a);

    REAL scale = sqrt(v.x * v.x + v.y * v.y + v.z * v.z); 
    Vector4 w = v / scale;
    Matrix4x4 wp = Matrix4x4(
                        0, -w.z, w.y, 0,
                        w.z, 0, -w.x, 0,
                        -w.y, w.x, 0, 0,
                        0,   0,   0,   0);
    Matrix4x4 wp2 = wp * wp;
    Matrix4x4 R = Matrix4x4::identity() + sina * wp + ocos * wp2;

    return getTransMat(p) * R * getInvTransMat(p); 
}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v)
{
    Matrix4x4 out = Matrix4x4(
                        1 / v.x, 0,       0,       0,
                        0,       1 / v.y, 0,       0,
                        0,       0,       1 / v.z, 0,
                        0,       0,       0,       1);

    return out;
}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v)
{
    Matrix4x4 out = Matrix4x4(
                        1, 0, 0, -v.x,
                        0, 1, 0, -v.y,
                        0, 0, 1, -v.z,
                        0, 0, 0, 1);

    return out;
}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat(const REAL radians)
{
    return getRotXMat(-radians);
}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat(const REAL radians)
{
    return getRotYMat(-radians);
}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat(const REAL radians)
{
    return getRotZMat(-radians);
}

// @returns the inverse rotation matrix around the vector and point by the specified angle
Matrix4x4 getInvRotMat(const Vector4 &p, const Vector4 &v, const REAL a) {
    return getRotMat(p, v, -a);
}

Matrix4x4 pose(const Vector4 &p, const Vector4 &look, const Vector4 &up)
{
    Matrix4x4 trans = getTransMat(p);
    Matrix4x4 rot = orient(look, up);

    return trans * rot;
}

Matrix4x4 orient(const Vector4 &look, const Vector4 &up)
{
    Vector4 n = -look.getNormalized();
    Vector4 v = (up - n.dot(up) * n).getNormalized();
    Vector4 u = (v.cross(n)).getNormalized();

    Matrix4x4 rot = Matrix4x4(
                        u.x, v.x, n.x, 0,
                        u.y, v.y, n.y, 0,
                        u.z, v.z, n.z, 0,
                        0,   0,   0,   1);

    return rot;
}
