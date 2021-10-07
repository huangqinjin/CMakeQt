#include "MainWindow.h"
#include <QtGui>
#include <QDebug>
#include <QSignalBlocker>
#include <Eigen/Geometry>
#include "ui_MainWindow.h"

using namespace Eigen;

class MainWindow::UI : public Ui::MainWindow
{
public:
    void setupUi(::MainWindow* w)
    {
        MainWindow::setupUi(w);
        T.matrix().setZero();
        zoomValue = zoomDefaultValue = zoomSlider->value();
        rotationValue = rotationDefaultValue = rotationSlider->value();
    }

    QPointF pos;
    Eigen::Affine3f T;

    int zoomValue, zoomDefaultValue;
    int rotationValue, rotationDefaultValue;
};

MainWindow::MainWindow(bool orth)
    : orth(orth), ui(new UI)
{
    ui->setupUi(this);
    connect(ui->resetButton, &QPushButton::clicked, [this] {
        initializeGL();
        update();
    });

    connect(ui->zoomSlider, &QSlider::valueChanged, [this](int value) {
        int dv = value - ui->zoomValue;
        if (this->orth) ui->T.linear() *= std::exp(dv * 0.04f);
        else ui->T.translation() += Vector3f(0, 0, dv * 0.1f);
        ui->zoomValue = value;
        update();
    });

    connect(ui->rotationSlider, &QSlider::valueChanged, [this](int value) {
        ui->T.linear() = AngleAxisf((ui->rotationValue - value) * 0.1f, Vector3f::UnitZ()) * ui->T.linear();
        ui->rotationValue = value;
        update();
    });
}

MainWindow::~MainWindow()
{

}

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
    if (!ui->T(3, 3)) glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (orth) glOrtho(-1, 1, -1, 1, 1, 3 /* -1 */);
    else glFrustum(-1.0, 1.0, -1.0, 1.0, 1, 100);
    glMatrixMode(GL_MODELVIEW);

    ui->T = Translation3f(0, 0, -2.f);
    {
        auto _ = QSignalBlocker(ui->zoomSlider);
        ui->zoomSlider->setValue(ui->zoomDefaultValue);
    }
    {
        auto _ = QSignalBlocker(ui->rotationSlider);
        ui->rotationSlider->setValue(ui->rotationDefaultValue);
    }
}

void MainWindow::resizeGL(int w, int h)
{
    (void)w;
    (void)h;
}

void MainWindow::paintGL()
{
    // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    {
        // Unlike GLUT, glViewport should be here
        auto sz = size();
        auto s = static_cast<GLsizei>(std::min(sz.width(), sz.height()));
        glViewport(0, 0, s, s);
    }

    
    glLoadMatrixf(ui->T.data());
    glTranslatef(-0.5f, -0.5f, -0.5f);

    GLfloat data[8][3] = {
        { 0, 0, 0 },
        { 1, 0, 0 },
        { 1, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 1 },
        { 0, 0, 1 },
        { 1, 0, 1 },
        { 1, 1, 1 },
    };

    glBegin(GL_QUADS);
    glColor3fv(data[0]); glVertex3fv(data[0]);
    glColor3fv(data[1]); glVertex3fv(data[1]);
    glColor3fv(data[2]); glVertex3fv(data[2]);
    glColor3fv(data[3]); glVertex3fv(data[3]);

    glColor3fv(data[2]); glVertex3fv(data[2]);
    glColor3fv(data[3]); glVertex3fv(data[3]);
    glColor3fv(data[4]); glVertex3fv(data[4]);
    glColor3fv(data[7]); glVertex3fv(data[7]);

    glColor3fv(data[4]); glVertex3fv(data[4]);
    glColor3fv(data[7]); glVertex3fv(data[7]);
    glColor3fv(data[6]); glVertex3fv(data[6]);
    glColor3fv(data[5]); glVertex3fv(data[5]);

    glColor3fv(data[6]); glVertex3fv(data[6]);
    glColor3fv(data[5]); glVertex3fv(data[5]);
    glColor3fv(data[0]); glVertex3fv(data[0]);
    glColor3fv(data[1]); glVertex3fv(data[1]);

    glColor3fv(data[0]); glVertex3fv(data[0]);
    glColor3fv(data[3]); glVertex3fv(data[3]);
    glColor3fv(data[4]); glVertex3fv(data[4]);
    glColor3fv(data[5]); glVertex3fv(data[5]);

    glColor3fv(data[1]); glVertex3fv(data[1]);
    glColor3fv(data[2]); glVertex3fv(data[2]);
    glColor3fv(data[7]); glVertex3fv(data[7]);
    glColor3fv(data[6]); glVertex3fv(data[6]);
    glEnd();

    // Draw the scene:
    //        glBegin(GL_TRIANGLES);
    //        glColor3f(1.0f, 0.0f, 0.0f);
    //        glVertex3f(-1, -1, 0.0);
    //
    //        glColor3f(0.0f, 1.0f, 0.0f);
    //        glVertex3f(1, 0.0, 0.0);
    //
    //        glColor3f(0.0f, 0.0f, 1.0f);
    //        glVertex3f(0.0, 1, 0.0);
    //        glEnd();
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
}

#if QT_VERSION_MAJOR < 6
#define position localPos
#endif

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    ui->pos = e->position();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MiddleButton)
    {
        initializeGL();
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    QPointF dp = e->position() - ui->pos;
    if (e->buttons() == Qt::LeftButton)
    {
        Vector3f axis(dp.y(), dp.x(), 0);
        ui->T.linear() = AngleAxisf(axis.norm() * 0.01f, axis.normalized()) * ui->T.linear();
    }
    else if(e->buttons() == Qt::RightButton)
    {
        Vector3f dir(dp.x(), -dp.y(), 0);
        ui->T.translation() += dir * 0.01f;
    }
    ui->pos = e->position();
    update();
}

#undef position

void MainWindow::wheelEvent(QWheelEvent* e)
{
}
