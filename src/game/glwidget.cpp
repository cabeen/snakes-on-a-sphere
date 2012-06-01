#include "glwidget.h"
#include "scene.h"
#include <QMouseEvent>
#include <qgl.h>
#ifdef __WIN32
#include <gl/glext.h>
#endif
#include <QTimer>
#include <QTime>
#include "drawengine.h"
#include "QApplication"

GlWidget::GlWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    m_prevx = m_prevy = 0.0;
    m_mdown = false;
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setAutoBufferSwap(false);
}

GlWidget::~GlWidget()
{
    delete m_engine;
    delete m_timer;
    delete m_time;
}

void GlWidget::mousePressEvent(QMouseEvent *event)
{
    m_mdown = true;
    m_prevx = event->x();
    m_prevy = event->y();
}

void GlWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mdown)
    {
        int nx = event->x();
        int ny = event->y();
        int dx = nx - m_prevx;
        int dy = ny - m_prevy;

        m_engine->mouse(dx, dy);

        m_prevx = nx;
        m_prevy = ny;
    }
}

void GlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_mdown)
    {
        int nx = event->x();
        int ny = event->y();
        int dx = nx - m_prevx;
        int dy = ny - m_prevy;

        m_engine->mouse(dx, dy);

        m_prevx = nx;
        m_prevy = ny;
    }

    m_mdown = false;
}

void GlWidget::wheelEvent(QWheelEvent *event)
{
    m_engine->wheel(event->delta());
}

void GlWidget::keyPressEvent(QKeyEvent *event)
{
    m_engine->press(event);

    switch (event->key())
    {
    case Qt::Key_Q:
    case Qt::Key_Escape:
        qApp->quit();
        break;
    }
}

void GlWidget::keyReleaseEvent(QKeyEvent *event)
{
    m_engine->release(event);
}

void GlWidget::initializeGL()
{
    m_engine = new DrawEngine(context());
    m_time = new QTime;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
    m_timer->start(30.0f);
}

void GlWidget::paintGL()
{
    Mode m = m_engine->getMode();
    float t = m_time->elapsed();
    QFont f("Dega Vu Sans Mono", 30, 6, false);
    QFont f2("Dega Vu Sans Mono", 18, 4, false);
    float fs = 0.4;

    if (m == Start)
    {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderText(width() * 0.3, height() / 2, QString("SNAKES ON A SPHERE"), f);
    }

    if (m == Play)
    {
        if (m_prevtime != -1) m_engine->update(t - m_prevtime);
        m_prevtime = t;
    }

    if (m != Start)
    {
        m_engine->draw(width(), height());
        renderText(10, 25, "Level: " + QString::number((int) m_engine->level()), f2);
        renderText(10, 85, "Lives: " + QString::number((int) m_engine->lives()), f2);
        renderText(10, 55, "Food left: " + QString::number((int) m_engine->left()), f2);
    }

    if (m != Play) m_prevtime = -1;


    if (m == Win)   renderText(width() * fs, height() / 2, QString("   YOU WON    "), f);
    if (m == Lost)  renderText(width() * fs, height() / 2, QString("GAME OVER, MAN"), f);
    if (m == Pause) renderText(width() * fs, height() / 2, QString("    PAUSED    "), f);
    if (m == Died)  renderText(width() * fs, height() / 2, QString("   YOU DIED   "), f);

    glFlush();
    swapBuffers();
}
