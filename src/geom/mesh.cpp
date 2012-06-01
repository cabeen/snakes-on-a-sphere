#include "mesh.h"
#include <qgl.h>
#include <iostream>

using namespace std;

Mesh::Mesh(int param1, int param2, float param3)
{
    m_param1 = param1;
    m_param2 = param2;
    m_param3 = param3;
    m_verts = NULL;
    m_faces = NULL;
    m_normals = NULL;
    m_texs = NULL;
}

Mesh::~Mesh()
{
    if (m_verts)
         delete[] m_verts;
    m_verts = NULL;

    if (m_faces)
         delete[] m_faces;
    m_faces = NULL;

    if (m_normals)
         delete[] m_normals;
    m_normals = NULL;

    if (m_texs)
         delete[] m_texs;
    m_texs = NULL;
}

void Mesh::glRender()
{
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < m_nfaces; i++) {
        int ridx = 3 * i;
        for (int j = 0; j < 3; j++) {
            int vidx = m_faces[ridx + j];

            int vxidx = 4 * vidx;
            int vyidx = vxidx + 1;
            int vzidx = vxidx + 2;

            int nxidx = 3 * vidx;
            int nyidx = nxidx + 1;
            int nzidx = nxidx + 2;

            int tsidx = 2 * vidx;
            int ttidx = tsidx + 1;

            glTexCoord2f(m_texs[tsidx], m_texs[ttidx]);
            glNormal3f(m_normals[nxidx], m_normals[nyidx], m_normals[nzidx]);
            glVertex3f(m_verts[vxidx], m_verts[vyidx], m_verts[vzidx]);
        }
    }
    glEnd();
}

void Mesh::glRenderNormals(Vector4 eye) {

    for (int i = 0; i < m_nverts; i++) {
        int vxidx = 4 * i;
        int vyidx = vxidx + 1;
        int vzidx = vxidx + 2;

        int nxidx = 3 * i;
        int nyidx = nxidx + 1;
        int nzidx = nxidx + 2;

        Vector4 direction = Vector4(m_normals[nxidx], m_normals[nyidx], m_normals[nzidx], 0);
        Vector4 vertex = Vector4(m_verts[vxidx], m_verts[vyidx], m_verts[vzidx], 1);

        Vector4 normalDirection = direction.getNormalized();

        // Draw a normal with a fixed length of 0.15
        glBegin(GL_LINES);
        glVertex3dv(vertex.data);
        glVertex3dv((vertex + normalDirection * 0.1).data);
        glEnd();

        // End the normal with an axis-aligned billboarded triangle (billboarding means always rotating
        // to face the camera, and axis-aligned means it can only rotate around the axis of the normal)
        Vector4 triangleVector = direction.cross(eye - vertex);
        if (triangleVector.getMagnitude2() > 1.0e-6f) {
            triangleVector = triangleVector.getNormalized() * 0.01;
            glBegin(GL_TRIANGLES);
            glVertex3dv((vertex + normalDirection * 0.15).data);
            glVertex3dv((vertex + normalDirection * 0.1 - triangleVector).data);
            glVertex3dv((vertex + normalDirection * 0.1 + triangleVector).data);
            glEnd();
        }
    }
}
