#include "MainWindow.h"
#include <QDebug>

MainWindow::MainWindow()
{
	setSurfaceType(OpenGLSurface);
	create();
	qDebug() << context.create();
	context.makeCurrent(this);
	initializeOpenGLFunctions();
}

void MainWindow::paintGL()
{
	context.makeCurrent(this);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	context.swapBuffers(this);
	context.doneCurrent();
}