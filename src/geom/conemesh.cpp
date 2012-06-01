#include "conemesh.h"
#include <iostream>
#include <math.h>

using namespace std;

ConeMesh::ConeMesh(int param1, int param2, float param3) : Mesh(param1, param2, param3)
{
    build();
}

void ConeMesh::setParams(int param1, int param2, float param3) {
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

void ConeMesh::build() {

    int nslices = m_param1 + 3;
    int nstacks = m_param2 + 1;
    m_nverts = nslices * nstacks + nslices + 2;
    m_nfaces = 2 * (nslices - 1) * (nstacks - 1) + 2 * (nslices - 1);
    m_texs = new float[m_nverts * 2];
    memset(m_texs, 0, 2 * m_nverts * sizeof(float));

    m_verts = new float[m_nverts * 4];
    m_normals = new float[m_nverts * 3];
    m_faces = new int[m_nfaces * 3];

    int topidx = m_nverts - 2;
    int bottomidx = m_nverts - 1;

    for (int i = 0; i < nslices; i++) {
        float s = (float) i / (float) (nslices - 1);
        float theta = 2 * 3.1415 * s;
        float cost = cos(theta);
        float sint = sin(theta);

        for (int j = 0; j < nstacks; j++) {
            int idx = nslices * j + i;
            int vidx = 4 * idx;
            int nidx = 3 * idx;

            float t = (float) j / (float) nstacks;
            float r = -0.5 * t + 0.5;

            m_verts[vidx + 0] = r * cost;
            m_verts[vidx + 1] = t - 0.5;
            m_verts[vidx + 2] = r * sint;
            m_verts[vidx + 3] = 1.0;

            int tidx = idx * 2;
            m_texs[tidx + 0] = s;
            m_texs[tidx + 1] = t;

            float sf = sqrt(5);
            float rc = 2 / sf;
            float vc = 1 / sf;
            m_normals[nidx + 0] = rc * cost;
            m_normals[nidx + 1] = vc; 
            m_normals[nidx + 2] = rc * sint;
        }

        int idx = nslices * nstacks + i;
        int vidx = 4 * idx;
        int nidx = 3 * idx;

        m_verts[vidx + 0] = 0.5 * cost;
        m_verts[vidx + 1] = -0.5;
        m_verts[vidx + 2] = 0.5 * sint;
        m_verts[vidx + 3] = 1.0;

        m_normals[nidx + 0] = 0;
        m_normals[nidx + 1] = -1;
        m_normals[nidx + 2] = 0;
    }

    int topvidx = 4 * topidx;
    int topnidx = 3 * topidx;

    m_verts[topvidx + 0] = 0.0;
    m_verts[topvidx + 1] = 0.5;
    m_verts[topvidx + 2] = 0.0;
    m_verts[topvidx + 3] = 1.0;

    m_normals[topnidx + 0] = 0.0;
    m_normals[topnidx + 1] = 1.0;
    m_normals[topnidx + 2] = 0.0;

    int bottomvidx = 4 * bottomidx;
    int bottomnidx = 3 * bottomidx;

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
        int bidxa = sidx + 1;
        int bidxb = sidx;
        int bidxc = bottomidx;

        m_faces[fidx++] = bidxb;
        m_faces[fidx++] = bidxa;
        m_faces[fidx++] = bidxc;

        int tidxa = nslices * (nstacks - 1) + i;
        int tidxb = nslices * (nstacks - 1) + i + 1;
        int tidxc = topidx;

        m_faces[fidx++] = tidxb;
        m_faces[fidx++] = tidxa;
        m_faces[fidx++] = tidxc;
    }
}


