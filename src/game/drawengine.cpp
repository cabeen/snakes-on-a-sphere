#include "drawengine.h"
#include "scene.h"
#include <QGLShaderProgram>
#include <QKeyEvent>
#include <QGLContext>

DrawEngine::DrawEngine(const QGLContext *context)
{
    m_lives = 5;
    m_level = 1;
    m_mode = Start;

    m_state = new State;
    m_state->init(1);

    m_ldown = m_rdown = false;
    m_scene = new Scene();

    this->m_context = context;

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1, -1);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

DrawEngine::~DrawEngine()
{
    delete m_scene;
    delete m_state;
}

void DrawEngine::draw(int w, int h)
{
    // Check for errors from the last frame
    int err;
    if ((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "GL is in an error state before painting.\n");
        fprintf(stderr, "(GL error code %d)\n", err);
        assert(0);
    }

    glViewport(0, 0, w, h);
    m_scene->render(m_state, w, h);
}

Mode DrawEngine::getMode()
{
    return m_mode;
}

int DrawEngine::lives()
{
    return m_lives;
}

void DrawEngine::setMode(Mode m)
{
    if (m_mode == Win)
    {
        m_level++;
        m_state->init(m_level);
    }
    if (m_mode == Start || m_mode == Lost)
    {
        m_level = 1;
        m_lives = 5;
        m_state->init(m_level);
    }
    if (m_mode == Died)
    {
        m_state->init(m_level);
    }

    m_mode = m;
}

void DrawEngine::update(float step)
{
    m_state->steer(m_ldown, m_rdown);
    bool nohit = m_state->update(step);

    if (!nohit && m_lives == 0)
    {
        m_mode = Lost;
    }
    else if (!nohit && m_lives > 0)
    {
        m_lives--;
        m_mode = Died;
    }
    else if (m_state->food.size())
    {
        m_mode = Play;
    }
    else
    {
        m_mode = Win;
    }
}

void DrawEngine::press(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_ldown = true;
        break;
    case Qt::Key_Right:
        m_rdown = true;
        break;
    case Qt::Key_B:
        m_scene->flipBackground();
        break;
    case Qt::Key_P:
    case Qt::Key_Space:
        flip();
        break;
    case Qt::Key_R:
        setMode(Start);
        break;
    }
}

void DrawEngine::release(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
        m_ldown = false;
        break;
    case Qt::Key_Right:
        m_rdown = false;
        break;
    }
}

int DrawEngine::left()
{
    return m_state->food.size();
}

int DrawEngine::level()
{
    return m_level;
}

void DrawEngine::flip()
{
    if (m_mode == Play)
        setMode(Pause);
    else
        setMode(Play);
}
