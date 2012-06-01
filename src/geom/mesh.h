#ifndef __MESH_H__
#define __MESH_H__
#define VDIM 4
#define NDIM 3
#define FDIM 3

#include <common.h>
#include <matrix.h>

class Mesh {
public:
    Mesh(int param1, int param2, float param3);  //contructor
    virtual ~Mesh();
    void glRender();
    void glRenderNormals(Vector4 eye);
    virtual void setParams(int param1, int param2, float param3) = 0;

protected:

    int m_param1;     /// The stack tesselation parameter
    int m_param2;     /// The slice tesselation parameter
    float m_param3;     /// The third tesselation parameter
    int m_nverts;     /// The number of vertices
    int m_nfaces;     /// The number of faces
    float* m_verts;   /// The vertex coordinates
    int* m_faces;     /// The face indices
    float* m_normals; /// The vertex normals
    float *m_texs;    /// The texture coordinates
};

#endif // __SHAPEMESH_H__
