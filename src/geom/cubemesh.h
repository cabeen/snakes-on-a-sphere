#ifndef __CUBE_H__
#define __CUBE_H__
#define NSIDES 6
#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define FRONT 4
#define BACK 5

#include <common.h>
#include <matrix.h>
#include <mesh.h>
#include <QVector>

class CubeMesh : public Mesh {
public:
    CubeMesh(int param1, int param2, float param3);  //contructor
    virtual ~CubeMesh();
    virtual void setParams(int param1, int param2, float param3);

private:
    //! Build the shape
    virtual void build();

    QVector<mat4<double>*> m_xfms; /// Transform to deform the unit square to each face
};

#endif // __CUBE_H__
