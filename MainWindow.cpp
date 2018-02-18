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

void MainWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}