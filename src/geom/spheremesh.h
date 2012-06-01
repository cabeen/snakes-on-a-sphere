#ifndef __SPHEREMESH_H__
#define __SPHEREMESH_H__

#include <matrix.h>
#include <common.h>
#include <mesh.h>
#include <QVector>

class SphereMesh : public Mesh {
public:
    SphereMesh(int param1, int param2, float param3);  //contructor
    virtual void setParams(int param1, int param2, float param3);

private:
    //! Build the shape
    virtual void build();
};

#endif // __SPHEREMESH_H__
