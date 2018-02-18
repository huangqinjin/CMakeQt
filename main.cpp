#include <iostream>
#include <QtCore>
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
public:
  //  MainWindow() : QOpenGLFunctions(this) {}

    void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

int main(int argc, char* argv[]) 
{
    QApplication app(argc, argv);
    std::cout << "Hello, World!" << std::endl;

    MainWindow w;
    w.show();

    return app.exec();
}