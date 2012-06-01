#ifndef __CYLINDERMESH_H__
#define __CYLINDERMESH_H__

#include <common.h>
#include <mesh.h>
#include <QVector>
#include <matrix.h>

class CylinderMesh : public Mesh {
public:
    CylinderMesh(int param1, int param2, float param3);  //contructor
    virtual void setParams(int param1, int param2, float param3);

private:
    virtual void build();
};

#endif // __CYLINDERMESH_H__
