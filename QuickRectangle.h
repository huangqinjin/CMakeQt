#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QQuickFramebufferObject>
#include <QMatrix4x4>
#include <Eigen/Geometry>

class QuickRectangle : public QQuickFramebufferObject
{
    Q_OBJECT

public:
    QuickRectangle();

    QMatrix4x4 transform() const;

public Q_SLOTS:
    void reset();
    void zoom(double value);
    void rotate(double value);
    void mousePressEvent(QObject* e);
    void mouseReleaseEvent(QObject* e);
    void mouseMoveEvent(QObject* e);
    void wheelEvent(QObject* e);

private:
    Renderer* createRenderer() const override;

private:
    const bool orth;
    QMatrix4x4 projection;
    Eigen::Affine3f modelview;
    QPointF pos;
    double zoomValue;
    double rotationValue;
};

#endif // RECTANGLE_H
