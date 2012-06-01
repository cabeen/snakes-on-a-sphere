#include "cylindermesh.h"

#include <iostream>
#include <math.h>

using namespace std;

CylinderMesh::CylinderMesh(int param1, int param2, float param3) : Mesh(param1, param2, param3)
{
    build();
}

void CylinderMesh::setParams(int param1, int param2, float param3) {
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

void CylinderMesh::build() {
    int nslices = m_param1 + 4;
    int nstacks = m_param2 + 2;
    m_nverts = nslices * nstacks + 2 * nslices + 2;
    m_nfaces = 2 * (nslices - 1) * (nstacks - 1) + 2 * (nslices - 1);

    m_verts = new float[m_nverts * 4];
    m_normals = new float[m_nverts * 3];
    m_faces = new int[m_nfaces * 3];
    m_texs = new float[m_nverts * 2];
    memset(m_texs, 0, 2 * m_nverts * sizeof(float));

    int topidx = m_nverts - 2;
    int bottomidx = m_nverts - 1;

    for (int i = 0; i < nslices; i++) {
        float s = (float) i / (float) (nslices - 1);
        float theta = 2 * 3.1415 * s;
        float x = 0.5 * cos(theta);
        float z = 0.5 * sin(theta);
        float mag = sqrt(x * x + z * z);
        float normalize = mag == 0 ? mag : 1 / mag;

        for (int j = 0; j < nstacks; j++) {
            int idx = nslices * j + i;

            float t = (float) j / (float) (nstacks - 1);
            float y = t - 0.5;

            int vidx = 4 * idx;
            m_verts[vidx + 0] = x;
            m_verts[vidx + 1] = y;
            m_verts[vidx + 2] = z;
            m_verts[vidx + 3] = 1.0;

            int tidx = 2 * idx;
            m_texs[tidx + 0] = s;
            m_texs[tidx + 1] = t;

            int nidx = 3 * idx;
            m_normals[nidx + 0] = normalize * x;
            m_normals[nidx + 1] = 0;
            m_normals[nidx + 2] = normalize * z;
        }

        float y = 0.5;

        int idx = nslices * nstacks + i;

        int vidxtop = 4 * idx;
        m_verts[vidxtop + 0] = x;
        m_verts[vidxtop + 1] = y;
        m_verts[vidxtop + 2] = z;
        m_verts[vidxtop + 3] = 1.0;

        int tidxtop = 2 * idx;
        m_texs[tidxtop + 0] = s;
        m_texs[tidxtop + 1] = 1.0;

        int nidxtop = 3 * idx;
        m_normals[nidxtop + 0] = 0;
        m_normals[nidxtop + 1] = 1;
        m_normals[nidxtop + 2] = 0;

        int vidxbot = 4 * (idx + nslices);
        m_verts[vidxbot + 0] = x;
        m_verts[vidxbot + 1] = -y;
        m_verts[vidxbot + 2] = z;
        m_verts[vidxbot + 3] = 1.0;

        int tidxbot = 2 * (idx + nslices);
        m_texs[tidxbot + 0] = s;
        m_texs[tidxbot + 1] = -1.0;

        int nidxbot = 3 * (idx + nslices);
        m_normals[nidxbot + 0] = 0;
        m_normals[nidxbot + 1] = -1;
        m_normals[nidxbot + 2] = 0;
    }

    int topvidx = 4 * topidx;
    int topnidx = 3 * topidx;
    int bottomvidx = 4 * bottomidx;
    int bottomnidx = 3 * bottomidx;

    m_verts[topvidx + 0] = 0.0;
    m_verts[topvidx + 1] = 0.5;
    m_verts[topvidx + 2] = 0.0;
    m_verts[topvidx + 3] = 1.0;

    m_normals[topnidx + 0] = 0.0;
    m_normals[topnidx + 1] = 1.0;
    m_normals[topnidx + 2] = 0.0;

    m_verts[bottomvidx + 0] = 0.0;
    m_verts[bottomvidx + 1] = -0.5;
    m_verts[bottomvidx + 2] = 0.0;
    m_verts[bottomvidx + 3] = 1.0;

    m_normals[bottomnidx + 0] = 0.0;
    m_normals[bottomnidx + 1] = -1.0;
    m_normals[bottomnidx + 2] = 0.0;

    int fidx = 0;
    for (int i = 0; i < nslices - 1; i++) {
        for (int j = 0; j < nstacks - 1; j++) {
            int idxa1 = nslices * j + i;
            int idxb1 = nslices * j + (i + 1);
            int idxc1 = nslices * (j + 1) + i;

            int idxa2 = nslices * (j + 1) + i;
            int idxb2 = nslices * j + (i + 1);
            int idxc2 = nslices * (j + 1) + (i + 1);

            m_faces[fidx++] = idxb1;
            m_faces[fidx++] = idxa1;
            m_faces[fidx++] = idxc1;
            m_faces[fidx++] = idxb2;
            m_faces[fidx++] = idxa2;
            m_faces[fidx++] = idxc2;
        }

        int sidx = nslices * nstacks + i;
        int tidxa = sidx;
        int tidxb = sidx + 1;
        int tidxc = topidx;

        m_faces[fidx++] = tidxb;
        m_faces[fidx++] = tidxa;
        m_faces[fidx++] = tidxc;

        int bidxa = tidxb + nslices;
        int bidxb = tidxa + nslices;
        int bidxc = bottomidx;

        m_faces[fidx++] = bidxb;
        m_faces[fidx++] = bidxa;
        m_faces[fidx++] = bidxc;

    }
}

