#include "spheremesh.h"

#include <iostream>
#include <math.h>

using namespace std;

SphereMesh::SphereMesh(int param1, int param2, float param3) : Mesh(param1, param2, param3)
{
    build();
}

void SphereMesh::setParams(int param1, int param2, float param3)
{
    if (m_param1 != param1 || m_param2 != param2) {
        m_param1 = param1;
        m_param2 = param2;
        m_param3 = param3;

        if (m_verts)
            delete[] m_verts;

        if (m_faces)
            delete[] m_faces;

        if (m_normals)
            delete[] m_normals;

        build();
    }
}

void SphereMesh::build()
{
    int nslices = m_param1 + 4;
    int nstacks = m_param2 + 3;
    m_nverts = nslices * nstacks;
    m_nfaces = 2 * (nslices - 1) * (nstacks - 1);

    m_verts = new float[m_nverts * 4];
    m_normals = new float[m_nverts * 3];
    m_faces = new int[m_nfaces * 3];
    m_texs = new float[m_nverts * 2];

    for (int i = 0; i < nslices; i++) {
        for (int j = 0; j < nstacks; j++) {
            int idx = nslices * j + i;
            int vidx = 4 * idx;
            int nidx = 3 * idx;
            int tidx = 2 * idx;

            float s = (float) i / (nslices - 1);
            float t = (float) j / (nstacks - 1);
            float theta = 2 * M_PI * s;
            float phi = M_PI * t;

            float x = 0.5 * cos(theta) * sin(phi);
            float y = 0.5 * cos(phi);
            float z = 0.5 * sin(theta) * sin(phi);

            m_verts[vidx + 0] = x;
            m_verts[vidx + 1] = y;
            m_verts[vidx + 2] = z;
            m_verts[vidx + 3] = 1.0;

            m_texs[tidx + 0] = s;
            m_texs[tidx + 1] = t;

            float normalize = 1 / sqrt(x * x + y * y + z * z);
            m_normals[nidx + 0] = normalize * x;
            m_normals[nidx + 1] = normalize * y;
            m_normals[nidx + 2] = normalize * z;
        }
    }

    int fidx = 0;
    for (int i = 0; i < nslices - 1; i++) {
        for (int j = 0; j < nstacks - 1; j++) {
            int idxa1 = nslices * j + i + 1;
            int idxc1 = nslices * j + i;
            int idxb1 = nslices * (j + 1) + i;

            int idxa2 = nslices * j + i + 1;
            int idxc2 = nslices * (j + 1) + i;
            int idxb2 = nslices * (j + 1) + (i + 1);

            m_faces[fidx++] = idxa1;
            m_faces[fidx++] = idxb1;
            m_faces[fidx++] = idxc1;
            m_faces[fidx++] = idxa2;
            m_faces[fidx++] = idxb2;
            m_faces[fidx++] = idxc2;
        }
    }
}
