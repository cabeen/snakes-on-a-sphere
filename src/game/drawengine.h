#ifndef DRAWENGINE_H
#define DRAWENGINE_H

#include "state.h"

class Scene;
class QGLContext;
class QKeyEvent;
class QGLShaderProgram;

enum Mode
{
    Start, Play, Pause, Win, Died, Lost
};

enum Shader
{
    Blinn, Toon, None
};

class DrawEngine
{
public:
    DrawEngine(const QGLContext *context);
    ~DrawEngine();
    void draw(int w, int h);
    void wheel(int delta) { m_state->camera(0, 0, delta);}
    void mouse(int dx, int dy) { m_state->camera(dx, dy, 0);}
    void press(QKeyEvent *event);
    void release(QKeyEvent *event);
    int left();
    void update(float step);
    Mode getMode();
    void setMode(Mode m);
    void flip();
    int level();
    int lives();

private:
    bool m_ldown;
    bool m_rdown;
    int m_lives;
    int m_level;
    Mode m_mode;
    State *m_state;
    Scene *m_scene;
    const QGLContext *m_context;
};

#endif // DRAWENGINE_H
