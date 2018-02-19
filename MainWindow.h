#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Compatibility>

class MainWindow : public QOpenGLWidget,
				   protected QOpenGLFunctions_3_3_Compatibility
{
	Q_OBJECT

public:
	MainWindow();
    ~MainWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* e) override;
    void keyReleaseEvent(QKeyEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
    class UI;
    const std::unique_ptr<UI> ui;
};