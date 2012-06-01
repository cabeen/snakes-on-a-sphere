#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPrinter>
#include <QPushButton>
#include <QtGui>

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class Canvas3D;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    virtual ~MainWindow();

private slots:
    void start();
    void exit();
    void about();
    void createElements();

private:
    Canvas3D *m_canvas;
};

#endif
