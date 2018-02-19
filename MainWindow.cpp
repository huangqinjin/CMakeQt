#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow()
{
}

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0);
}

void MainWindow::resizeGL(int w, int h)
{
    (void)w;
    (void)h;
    update();
}

void MainWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1, -1, 0.0);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1, 0.0, 0.0);
    
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0, 1, 0.0);
    glEnd();
}