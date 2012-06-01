#ifndef CAMERA_H
#define CAMERA_H

#include <algebra.h>
#include "camera.h"

class Camera
{
public:

    Camera();

    Matrix4x4 getProjectionMatrix() const;
    Matrix4x4 getModelviewMatrix() const;

    Vector4 getPosition() const;
    Vector4 getLook() const;
    Vector4 getUp() const;

    float getAspectRatio() const;
    float getHeightAngle() const;
    bool getOrtho() const;

    void orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up);
    void translate(const Vector4 &v);

    void rotateU(float degrees);
    void rotateV(float degrees);
    void rotateN(float degrees);

    void setHeightAngle(float h);
    void setAspectRatio(float aspectRatio);
    void setClip(float nearPlane, float farPlane);
    void setOrtho(bool ortho);

private:
    virtual void update();

    bool m_ortho;
    Vector4 m_eye;
    Vector4 m_look;
    Vector4 m_up;
    REAL m_aspectratio;
    REAL m_heightangle;
    REAL m_near;
    REAL m_far;
    Vector4 m_u;
    Vector4 m_v;
    Vector4 m_n;
    Matrix4x4 m_view;
    Matrix4x4 m_proj;
};

#endif // CAMERA_H
