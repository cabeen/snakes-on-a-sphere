#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "state.h"
#include "algebra.h"
#include "camera.h"
#include "mesh.h"

#define NUM_OPENGL_LIGHTS 8

using namespace std;

struct Color
{
    union {
        struct {
           float r;
           float g;
           float b;
           float a;
        };
        float channels[4];
    };
};

struct Light
{
   int id;
   Color color;
   Vector4  pos;
   Vector4 dir;
   Vector4 function;
};

struct GlobalLight
{
   float ka;
   float kd;
   float ks;
   float kt;
};

struct Material
{
   Color cAmbient;
   Color cDiffuse;
   Color cReflective;
   Color cSpecular;
   float shininess;

   bool usetex;
   QString tex;
};

struct Element
{
    bool leaf;
    Matrix4x4 xfm;
    Material mat;

    Mesh *mesh;
    vector<Element*> children;
};

class Scene
{
public:
    Scene();
    virtual ~Scene();
    void render(State *state, int w, int h);
    void flipBackground();

private:
    void deleteElement(Element *element);
    void renderElement(const Matrix4x4 &xfm, Element *elem);

    Element *m_root;
    Element *m_pfood;
    Element *m_pjoints;
    Element *m_food;
    Element *m_joint;
    Element *m_world;
    Element *m_barrier;
    Element *m_pbarrier;
    Mesh *m_ssphere;
    Mesh *m_bsphere;

    bool m_background;
    int m_skybox;
    Camera *m_camera;
    vector<QString> m_cube;
    vector<Light> m_lights;
    map<QString, int> m_textures;
};

#endif // SCENE_H
