#include "cubemesh.h"
#include <iostream>

using namespace std;

CubeMesh::CubeMesh(int param1, int param2, float param3) : Mesh(param1, param2, param3)
{
    m_xfms = QVector<mat4<double>*>(NSIDES);
    m_xfms[FRONT] = new mat4<double>(1, 0, 0, 0,
                                     0, 1, 0, 0,
                                     0, 0, 1, 0.5,
                                     0, 0, 0, 1);
    m_xfms[BACK] = new mat4<double>(-1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, -1, -0.5,
                                    0, 0, 0, 1);
    m_xfms[RIGHT] = new mat4<double>(0, 0, 1, 0.5,
                                     0, -1, 0, 0,
                                     1, 0, 0, 0,
                                     0, 0, 0, 1);
    m_xfms[LEFT] = new mat4<double>(0, 0, -1, -0.5,
                                    0, 1, 0, 0,
                                    1, 0, 0, 0,
                                    0, 0, 0, 1);
    m_xfms[TOP] = new mat4<double>(1, 0, 0, 0,
                                   0, 0, 1, 0.5,
                                   0, -1, 0, 0,
                                   0, 0, 0, 1);
    m_xfms[BOTTOM] = new mat4<double>(1, 0, 0, 0,
                                      0, 0, -1, -0.5,
                                      0, 1, 0, 0,
                                      0, 0, 0, 1);
    build();
}

CubeMesh::~CubeMesh()
{
    for (int i = 0; i < NSIDES; i++) {
        mat4<double>* xfm = m_xfms[i];
        if (xfm)
            delete xfm;
        xfm = NULL;
    }
}

void CubeMesh::setParams(int param1, int param2, float param3) {
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

void CubeMesh::build() {

    int nvertEdge1 = m_param1 + 1;
    int nvertEdge2 = m_param2 + 1;
    int nvertsSide = nvertEdge1 * nvertEdge2;
    int nfacesSide = 2 * m_param1 * m_param2;
    m_nverts = NSIDES * nvertsSide;
    m_nfaces = NSIDES * nfacesSide;
    int flen = m_nfaces * 3;

    m_verts = new float[m_nverts * 4];
    m_normals = new float[m_nverts * 3];
    m_faces = new int[flen];
    m_texs = new float[m_nverts * 2];
    memset(m_texs, 0, 2 * m_nverts * sizeof(float));

    vec4<double>* in = new vec4<double>();
    vec4<double>* out = new vec4<double>();
    vec4<double>* normal = new vec4<double>();
    normal->x = 0.0;
    normal->y = 0.0;
    normal->z = 1.0;
    normal->w = 0.0;

    for (int i = 0; i < nvertEdge1; i++) {
        for (int j = 0; j < nvertEdge2; j++) {
            float s = (float) i / m_param1;
            float t = (float) j / m_param2;
            in->x = s - 0.5;
            in->y = t - 0.5;
            in->z = 0.0;
            in->w = 1.0;

            int sidx = nvertEdge1 * j + i;

            for (int k = 0; k < NSIDES; k++) {
                int idx = k * nvertsSide + sidx;

                m_xfms[k]->mulVec4(*in, *out);

                int vidx = idx * 4;
                m_verts[vidx + 0] = out->x;
                m_verts[vidx + 1] = out->y;
                m_verts[vidx + 2] = out->z;
                m_verts[vidx + 3] = out->w;

                int tidx = idx * 2;
                m_texs[tidx + 0] = s;
                m_texs[tidx + 1] = t;

                m_xfms[k]->mulVec4(*normal, *out);

                int nidx = idx * 3;
                m_normals[nidx + 0] = out->x;
                m_normals[nidx + 1] = out->y;
                m_normals[nidx + 2] = out->z;
            }
        }
    }

    int fcounter = 0;
    for (int i = 0; i < m_param1; i++) {
        for (int j = 0; j < m_param2; j++) {
            int idxa1 = nvertEdge1 * j + i;
            int idxb1 = nvertEdge1 * j + (i + 1);
            int idxc1 = nvertEdge1 * (j + 1) + i;
            int idxa2 = nvertEdge1 * (j + 1) + i;
            int idxb2 = nvertEdge1 * j + (i + 1);
            int idxc2 = nvertEdge1 * (j + 1) + (i + 1);

            for (int k = 0; k < NSIDES; k++) {
                int vsidx = k * nvertsSide;
                m_faces[fcounter++] = vsidx + idxa1;
                m_faces[fcounter++] = vsidx + idxb1;
                m_faces[fcounter++] = vsidx + idxc1;
                m_faces[fcounter++] = vsidx + idxa2;
                m_faces[fcounter++] = vsidx + idxb2;
                m_faces[fcounter++] = vsidx + idxc2;
            }
        }
    }

    delete in;
    delete out;
    delete normal;
}
