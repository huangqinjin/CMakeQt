#include "QuickRectangle.h"
#include <QDebug>
#include <QQuickWindow>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QSignalBlocker>

using namespace Eigen;

template<typename T, typename C>
int offset(T C::* p)
{
    return static_cast<int>(reinterpret_cast<long long>(
           &(static_cast<const C*>(nullptr)->*p)));
}

template<typename T, int N>
int size(const T (&)[N])
{
    return N;
}

class RectangleRenderer : public QQuickFramebufferObject::Renderer,
                          public QOpenGLFunctions
{
    QOpenGLVertexArrayObject vertexArray;
    QOpenGLBuffer vertexBuffer;
    QOpenGLShaderProgram program;
    int vertexAttr;
    int colorAttr;
    int matrixUniform;
    QMatrix4x4 transform;

public:
    RectangleRenderer()
        : vertexBuffer(QOpenGLBuffer::VertexBuffer)
    {
        initializeOpenGLFunctions();

        const char* vsrc = R"(
uniform mediump mat4 matrix;
attribute vec4 vertex;
attribute vec4 color;
varying vec4 fcolor;
void main()
{
    fcolor = color;
    gl_Position = matrix * vertex;
}
)";

        const char *fsrc = R"(
varying vec4 fcolor;
void main()
{
    gl_FragColor = fcolor;
}
)";

        program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
        program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
        program.link();

        matrixUniform = program.uniformLocation("matrix");
        vertexAttr = program.attributeLocation("vertex");
        colorAttr = program.attributeLocation("color");

        struct VertexData
        {
            GLfloat color[3];
            GLfloat position[3];
        };

        VertexData data[] = {
            {{1.0f, 0.0f, 0.0f}, {-1, -1, 0.0}},
            {{0.0f, 1.0f, 0.0f}, {1, 0.0, 0.0}},
            {{0.0f, 0.0f, 1.0f}, {0.0, 1, 0.0}},
        };

        vertexBuffer.create();
        vertexBuffer.bind();
        vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
        vertexBuffer.allocate(data, sizeof(data));

        vertexArray.create();
        QOpenGLVertexArrayObject::Binder _(&vertexArray);

        program.setAttributeBuffer(colorAttr, GL_FLOAT,
                                   offset(&VertexData::color),
                                   size(data[0].color),
                                   sizeof(VertexData));
        program.setAttributeBuffer(vertexAttr, GL_FLOAT,
                                   offset(&VertexData::position),
                                   size(data[0].position),
                                   sizeof(VertexData));

        program.enableAttributeArray(colorAttr);
        program.enableAttributeArray(vertexAttr);
    }

    void render() override
    {

        glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        program.bind();
        vertexArray.bind();
        program.setUniformValue(matrixUniform, transform);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        vertexArray.release();
        program.release();

        glDisable(GL_DEPTH_TEST);
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        format.setSamples(4);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject* f) override
    {
        if (auto r = qobject_cast<QuickRectangle*>(f))
        {
            transform = r->transform();
        }
    }
};

QuickRectangle::QuickRectangle() : orth(true)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    if (orth) projection.ortho(-1, 1, -1, 1, 1, 3 /* -1 */);
    else projection.frustum(-1.0, 1.0, -1.0, 1.0, 1, 100);
    reset();
}

QQuickFramebufferObject::Renderer* QuickRectangle::createRenderer() const
{
    return new RectangleRenderer;
}

QMatrix4x4 QuickRectangle::transform() const
{
    QMatrix4x4 m(modelview.cast<float>().data());
    // TODO
    // return projection * m;
    return m;
}

void QuickRectangle::reset()
{
    zoomValue = 0;
    rotationValue = 0;
    if(QQuickWindow* const w = window())
    {
        // auto _ = QSignalBlocker(w);
        w->setProperty("zoomValue", zoomValue);
        w->setProperty("rotationValue", rotationValue);
    } 
    modelview = Translation3f(0, 0, -2.f);
    update();
}

void QuickRectangle::zoom(double value)
{
    double dv = value - zoomValue;
    if (this->orth) modelview.linear() *= std::exp(dv * 0.04f);
    else modelview.translation() += Vector3f(0, 0, dv * 0.1f);
    zoomValue = value;
    update();
}

void QuickRectangle::rotate(double value)
{
    modelview.linear() = AngleAxisf((rotationValue - value) * 0.1f, Vector3f::UnitZ()) * modelview.linear();
    rotationValue = value;
    update();
}

#if QT_VERSION_MAJOR < 6
#define position localPos
#endif

void QuickRectangle::mousePressEvent(QMouseEvent* e)
{
    pos = e->position();
}

void QuickRectangle::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::MiddleButton)
    {
        reset();
    }
}

void QuickRectangle::mouseMoveEvent(QMouseEvent* e)
{
    QPointF dp = e->position() - this->pos;
    if (e->buttons() == Qt::LeftButton)
    {
        Vector3f axis(dp.y(), dp.x(), 0);
        modelview.linear() = AngleAxisf(axis.norm() * 0.01f, axis.normalized()) * modelview.linear();
    }
    else if(e->buttons() == Qt::RightButton)
    {
        Vector3f dir(dp.x(), -dp.y(), 0);
        modelview.translation() += dir * 0.01f;
    }
    this->pos = e->position();
    update();
}

#undef position

void QuickRectangle::wheelEvent(QWheelEvent* e)
{
}