#include "scene.h"
#include "camera.h"
#include <QFile>
#include "spheremesh.h"
#include <qgl.h>
#include "cubemesh.h"
#include "cylindermesh.h"
#include "conemesh.h"

Scene::Scene()
{
    m_bsphere = new SphereMesh(32, 32, 0);
    m_ssphere = new SphereMesh(16, 16, 0);

    // MATERIALS
    Material bmat;
    bmat.cAmbient.r = bmat.cAmbient.g = bmat.cAmbient.b = .1;
    bmat.cDiffuse.r = bmat.cDiffuse.g = bmat.cDiffuse.b = .5;
    bmat.cSpecular.r = bmat.cSpecular.g = bmat.cSpecular.b = 1;
    bmat.shininess = 64;
    bmat.usetex = true;
    bmat.tex = QString(":/data/textures/rock.jpg");

    Material fmat;
    fmat.cAmbient.r = fmat.cAmbient.g = fmat.cAmbient.b = .1;
    fmat.cDiffuse.r = fmat.cDiffuse.g = fmat.cDiffuse.b = .5;
    fmat.cSpecular.r = fmat.cSpecular.g = fmat.cSpecular.b = 1;
    fmat.shininess = 64;
    fmat.usetex = false;

    Material wmat;
    wmat.cAmbient.r = wmat.cAmbient.g = wmat.cAmbient.b = .1;
    wmat.cDiffuse.r = wmat.cDiffuse.g = wmat.cDiffuse.b = 0.5;
    wmat.cSpecular.r = wmat.cSpecular.g = wmat.cSpecular.b = 1;
    wmat.shininess = 64;
    wmat.usetex = true;
    wmat.tex = QString(":/data/textures/mars.png");

    Material jmat;
    jmat.cAmbient.r = jmat.cAmbient.g = jmat.cAmbient.b = .1;
    jmat.cDiffuse.r = jmat.cDiffuse.g = jmat.cDiffuse.b = .4;
    jmat.cSpecular.r = jmat.cSpecular.g = jmat.cSpecular.b = 1;
    jmat.shininess = 64;
    jmat.usetex = true;
    jmat.tex = QString(":/data/textures/snake.jpg");

    // SCENE ELEMENTS

    Element *foodp = new Element;
    foodp->leaf = true;
    foodp->mesh = m_ssphere;
    foodp->mat = fmat;

    m_food = new Element;
    m_food->leaf = false;
    m_food->xfm = m_food->xfm.identity();
    m_food->children.push_back(foodp);

    m_pfood = new Element;
    m_pfood->leaf = false;
    m_pfood->xfm = m_pfood->xfm.identity();

    Element *barrierp = new Element;
    barrierp->leaf = true;
    barrierp->mesh = m_ssphere;
    barrierp->mat = bmat;

    m_barrier = new Element;
    m_barrier->leaf = false;
    m_barrier->xfm = m_barrier->xfm.identity();
    m_barrier->children.push_back(barrierp);

    m_pbarrier = new Element;
    m_pbarrier->leaf = false;
    m_pbarrier->xfm = m_pbarrier->xfm.identity();

    Element *jointp = new Element;
    jointp->leaf = true;
    jointp->mesh = m_ssphere;
    jointp->mat = jmat;

    Element *jointr = new Element;
    jointr->leaf = false;
    jointr->xfm = getRotXMat(M_PI / 2.0);
    jointr->children.push_back(jointp);

    m_joint = new Element;
    m_joint->leaf = false;
    m_joint->xfm = m_joint->xfm.identity();
    m_joint->children.push_back(jointr);

    m_pjoints = new Element;
    m_pjoints->leaf = false;
    m_pjoints->xfm = m_pjoints->xfm.identity();

    Element *worldp = new Element;
    worldp->leaf = true;
    worldp->mesh = m_bsphere;
    worldp->mat = wmat;

    m_world = new Element;
    m_world->leaf = false;
    m_world->xfm = m_world->xfm.identity();
    m_world->children.push_back(worldp);

    m_root = new Element();
    m_root->leaf = false;
    m_root->xfm = m_root->xfm.identity();
    m_root->children.push_back(m_world);
    m_root->children.push_back(m_pjoints);
    m_root->children.push_back(m_pfood);
    m_root->children.push_back(m_pbarrier);

    // CAMERA

    m_camera = new Camera();
    m_camera->orientLook(Vector4(3, 0, 0, 1), Vector4(-3, 0, 0, 1), Vector4(0, 1, 0, 1));

    // LIGHTS

    int id = 0;
    for (int i = -1; i <= 1; i += 2)
    {
        Light light;
        Color color;
        color.r = color.g = color.b = color.a = 1.0f;
        light.color = color;
        light.id = id++;
        light.pos = Vector4(3.0 * i, 3.0 * i, 3.0 * i, 1.0);

        m_lights.push_back(light);
    }

    // SKYBOX

    m_background = true;

    m_cube.push_back(QString(":/data/textures/astra/posx.jpg"));
    m_cube.push_back(QString(":/data/textures/astra/negx.jpg"));
    m_cube.push_back(QString(":/data/textures/astra/posy.jpg"));
    m_cube.push_back(QString(":/data/textures/astra/negy.jpg"));
    m_cube.push_back(QString(":/data/textures/astra/posz.jpg"));
    m_cube.push_back(QString(":/data/textures/astra/negz.jpg"));

    for (unsigned i = 0; i < m_cube.size(); i++)
    {
        QString fn = m_cube[i];
        QImage image;
        image.load(fn);
        image = image.mirrored(false, true);
        QImage texture = QGLWidget::convertToGLFormat(image);

        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glBindTexture(GL_TEXTURE_2D, 0);

        m_textures[fn] = id;
    }
}

Scene::~Scene()
{
    delete m_camera;

    delete m_ssphere;
    delete m_bsphere;

    deleteElement(m_root);
}

void Scene::flipBackground()
{
    m_background = !m_background;
}

void Scene::deleteElement(Element *element)
{
    // Delete children...
    if (element)
        delete element;
    element = NULL;
}

void Scene::render(State *state, int w, int h)
{
    m_camera->setAspectRatio((float) w / (float) h);
    m_camera->orientLook(state->eye, state->look, state->up);

    double fs = state->foodrad / 0.5;
    double js = state->jointrad / 0.5;
    double ws = state->worldrad / 0.5;
    double bs = state->barrierrad / 0.5;

    m_food->xfm = getScaleMat(Vector4(fs, fs, fs, 1));
    m_joint->xfm = getScaleMat(Vector4(js, js, js, 1));
    m_world->xfm = getScaleMat(Vector4(ws, ws, ws, 1));
    m_barrier->xfm = getScaleMat(Vector4(bs, bs, bs, 1));

    // Delete children...
    m_pfood->children.clear();
    for (int i = 0; i < state->food.size(); i++) {
        Element *tfood = new Element;
        tfood->leaf = false;
        // set pose also
        tfood->xfm = getTransMat(state->food[i]);
        tfood->children.push_back(m_food);

        m_pfood->children.push_back(tfood);
    }

    // Delete children
    m_pbarrier->children.clear();
    for (int i = 0; i < state->barrier.size(); i++) {
        Element *tbarrier = new Element;
        tbarrier->leaf = false;
        // set pose also
        tbarrier->xfm = getTransMat(state->barrier[i]);
        tbarrier->children.push_back(m_barrier);

        m_pbarrier->children.push_back(tbarrier);
    }

    // Delete children...
    m_pjoints->children.clear();
    for (int i = 0; i < state->joints.size(); i++) {
        Element *tjoint = new Element;
        tjoint->leaf = false;
        // set pose also
        Pose jp = state->joints[i];
        tjoint->xfm = pose(jp.pos, jp.dir, jp.pos);
       // tjoint->xfm = getTransMat(state->joints[i].pos);
        tjoint->children.push_back(m_joint);

        m_pjoints->children.push_back(tjoint);
    }

    double matrix[16];
    m_camera->getProjectionMatrix().getTranspose().fillArray(matrix);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);

    m_camera->getModelviewMatrix().getTranspose().fillArray(matrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(matrix);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_background)
    {
        glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);

        glColor4f(1,1,1,1);
        float extent = 40.0f;

        glBindTexture(GL_TEXTURE_2D, m_textures[m_cube[0]]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f( -extent,  extent, -extent );
        glTexCoord2f(0, 1); glVertex3f( -extent, -extent, -extent );
        glTexCoord2f(1, 1); glVertex3f(  extent, -extent, -extent );
        glTexCoord2f(1, 0); glVertex3f(  extent,  extent, -extent );
        glEnd();

        glBindTexture(GL_TEXTURE_2D, m_textures[m_cube[1]]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  extent,  extent,  extent );
        glTexCoord2f(0, 1); glVertex3f(  extent, -extent,  extent );
        glTexCoord2f(1, 1); glVertex3f( -extent, -extent,  extent );
        glTexCoord2f(1, 0); glVertex3f( -extent,  extent,  extent );
        glEnd();

        glBindTexture(GL_TEXTURE_2D, m_textures[m_cube[2]]);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0); glVertex3f(  extent,  extent,  extent );
        glTexCoord2f(1, 1); glVertex3f( -extent,  extent,  extent );
        glTexCoord2f(0, 1); glVertex3f( -extent,  extent, -extent );
        glTexCoord2f(0, 0); glVertex3f(  extent,  extent, -extent );
        glEnd();

        glBindTexture(GL_TEXTURE_2D, m_textures[m_cube[3]]);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 0); glVertex3f( -extent, -extent, -extent );
        glTexCoord2f(0, 0); glVertex3f( -extent, -extent,  extent );
        glTexCoord2f(0, 1); glVertex3f(  extent, -extent,  extent );
        glTexCoord2f(1, 1); glVertex3f(  extent, -extent, -extent );
        glEnd();

        glBindTexture(GL_TEXTURE_2D, m_textures[m_cube[4]]);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3f( -extent, -extent, -extent );
        glTexCoord2f(1, 0); glVertex3f( -extent,  extent, -extent );
        glTexCoord2f(0, 0); glVertex3f( -extent,  extent,  extent );
        glTexCoord2f(0, 1); glVertex3f( -extent, -extent,  extent );
        glEnd();

        glBindTexture(GL_TEXTURE_2D, m_textures[m_cube[5]]);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(  extent,  extent, -extent );
        glTexCoord2f(0, 1); glVertex3f(  extent, -extent, -extent );
        glTexCoord2f(1, 1); glVertex3f(  extent, -extent,  extent );
        glTexCoord2f(1, 0); glVertex3f(  extent,  extent,  extent );
        glEnd();

        glPopAttrib();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_BLEND);
    }

    for (int i = 0; i < m_lights.size(); i++) {

        Light light = m_lights[i];
        float zero[] = { 0, 0, 0, 0 };
        int id = GL_LIGHT0 + light.id;

        if (!glIsEnabled(id)) {
            glLightfv(id, GL_AMBIENT, zero);
            glLightfv(id, GL_DIFFUSE, light.color.channels);
            glLightfv(id, GL_SPECULAR, light.color.channels);

            float position[] = { light.pos.data[0], light.pos.data[1], light.pos.data[2], 1.0f };
            glLightfv(id, GL_POSITION, position);
            glEnable(id);
        }
    }

    glMatrixMode(GL_MODELVIEW);
    renderElement(m_root->xfm.identity(), m_root);
}

void Scene::renderElement(const Matrix4x4 &xfm, Element *elem)
{
    if (elem->leaf)
    {
        Material material = elem->mat;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material.cAmbient.channels);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material.cDiffuse.channels);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material.cSpecular.channels);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material.shininess);

        if (material.usetex)
        {
            if (m_textures.count(material.tex) == 0)
            {
                QImage image;
                image.load(material.tex);
                image = image.mirrored(false, true);
                QImage texture = QGLWidget::convertToGLFormat(image);

                GLuint id;
                glGenTextures(1, &id);
                glBindTexture(GL_TEXTURE_2D, id);
                gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.width(), texture.height(), GL_RGBA, GL_UNSIGNED_BYTE, texture.bits());
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glBindTexture(GL_TEXTURE_2D, 0);

                m_textures[material.tex] = id;
            }
            else
            {
                int id = m_textures[material.tex];
                glBindTexture(GL_TEXTURE_2D, id);
            }
        }

        double matrix[16];
        glPushMatrix();
        xfm.getTranspose().fillArray(matrix);
        glMultMatrixd(matrix);
        elem->mesh->glRender();
        glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        Matrix4x4 nxfm = xfm * elem->xfm;
        for (int i = 0; i < elem->children.size(); i++)
            renderElement(nxfm, elem->children[i]);
    }
}

