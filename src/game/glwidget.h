#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <algebra.h>
#include <QGLShaderProgram>
#include <QGLShader>
#include "drawengine.h"

class GlWidget : public QGLWidget
{
    Q_OBJECT
public:
    GlWidget(QWidget *parent = 0);
    virtual ~GlWidget();

protected:
    virtual void initializeGL();
    virtual void paintGL();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

    float m_prevtime;
    bool m_mdown;
    float m_prevx;
    float m_prevy;
    QTimer *m_timer;
    QTime *m_time;
    DrawEngine *m_engine;
};

#endif // GLWIDGET_H
