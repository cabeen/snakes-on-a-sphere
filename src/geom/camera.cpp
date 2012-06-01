#include "camera.h"
#include <qgl.h>
#include "common.h"

Camera::Camera() {
    m_ortho = false;
    m_eye = Vector4(0.0, 0.0, 2.0, 1.0);
    m_look = Vector4(0.0, 0.0, -1.0, 1.0);
    m_up = Vector4(0.0, 1.0, 0.0, 1.0);
    m_aspectratio = 1.0;
    m_heightangle = 45.0;
    m_near = 0.1;
    m_far = 100.0;
    m_u = Vector4();
    m_v = Vector4();
    m_n = Vector4();
    m_view = Matrix4x4::identity();
    m_proj = Matrix4x4::identity();
    update();
}

void Camera::setOrtho(bool ortho) {
    m_ortho = ortho;
    update();
}

bool Camera::getOrtho() const {
    return m_ortho;
}

void Camera::setAspectRatio(float a)
{
    m_aspectratio = a;
    update();
}

Matrix4x4 Camera::getProjectionMatrix() const
{
    return m_proj;
}

Matrix4x4 Camera::getModelviewMatrix() const
{
    return m_view;
}

Vector4 Camera::getPosition() const
{
    return m_eye;
}

Vector4 Camera::getLook() const
{
    return m_look;
}

Vector4 Camera::getUp() const
{
    return m_up;
}

float Camera::getAspectRatio() const
{
    return m_aspectratio;
}

float Camera::getHeightAngle() const
{
    return m_heightangle;
}

void Camera::orientLook(const Vector4 &eye, const Vector4 &look, const Vector4 &up)
{
    m_eye = eye;
    m_look = look;
    m_up = up;

    update();
}

void Camera::setHeightAngle(float h)
{
    m_heightangle = h;
    update();
}

void Camera::translate(const Vector4 &v)
{
    m_eye += v.x * m_u;
    m_eye += v.y * m_v;
    m_eye += v.z * m_n;
    update();
}

void Camera::rotateU(float degrees)
{
    REAL rad = degrees * M_PI / 180.f;
    Matrix4x4 m = getRotMat(m_u, rad);
    m_look = m * m_look;
    m_up = m * m_up;
    update();
}

void Camera::rotateV(float degrees)
{
    REAL rad = degrees * M_PI / 180.f;
    Matrix4x4 m = getRotMat(m_v, rad);
    m_look = m * m_look;
    m_up = m * m_up;
    update();
}

void Camera::rotateN(float degrees)
{
    REAL rad = degrees * M_PI / 180.f;
    Matrix4x4 m = getRotMat(m_n, rad);
    m_look = m * m_look;
    m_up = m * m_up;
    update();
} 
void Camera::setClip(float nearPlane, float farPlane)
{
    m_near = nearPlane;
    m_far = farPlane;
    update();
}

void Camera::update() {

    Vector3 look = Vector3(m_look.x, m_look.y, m_look.z);
    Vector3 up = Vector3(m_up.x, m_up.y, m_up.z);
    Vector3 n = -look.getNormalized();
    Vector3 v = (up - n.dot(up) * n).getNormalized(); 
    Vector3 u = (v.cross(n)).getNormalized(); 

    Matrix4x4 m5 = getInvTransMat(m_eye);
    Matrix4x4 m4 = Matrix4x4(
                        u.x, u.y, u.z, 0,
                        v.x, v.y, v.z, 0,
                        n.x, n.y, n.z, 0,
                        0,   0,   0,   1);
    m_view = m4 * m5;
  
    if (m_ortho) {
        REAL ha = m_heightangle * M_PI / 180.f;;
        REAL h = tan(ha / 2.0);
        REAL w = m_aspectratio * h;
        REAL sx = w / 2.0;
        REAL sy = h / 2.0;
        REAL sz = m_far;

        Matrix4x4 m3 = getInvScaleMat(Vector4(sx, sy, sz, 1));
        Matrix4x4 m2 = Matrix4x4(
                            1, 0, 0, 0,
                            0, 1, 0, 0,
                            0, 0, 0, 0,
                            0, 0, 0, 1);

        m_proj = m2 * m3;
    } else {
        REAL ha = m_heightangle * M_PI / 180.f;;
        REAL h = tan(ha / 2.0);
        REAL w = m_aspectratio * h;
        REAL sx = w * m_far;
        REAL sy = h * m_far;
        REAL sz = m_far;

        REAL c = -m_near / m_far;

        Matrix4x4 m3 = getInvScaleMat(Vector4(sx, sy, sz, 1));
        Matrix4x4 m2 = Matrix4x4(
                            1, 0, 0,            0,
                            0, 1, 0,            0,
                            0, 0, -1 / (c + 1), c / (c + 1),
                            0, 0, -1,           0);

        m_proj = m2 * m3;
    }

    m_look.x = look.x;
    m_look.y = look.y;
    m_look.z = look.z;

    m_up.x = up.x;
    m_up.y = up.y;
    m_up.z = up.z;

    m_n.x = n.x;
    m_n.y = n.y;
    m_n.z = n.z;

    m_v.x = v.x;
    m_v.y = v.y;
    m_v.z = v.z;

    m_u.x = u.x;
    m_u.y = u.y;
    m_u.z = u.z;
}
