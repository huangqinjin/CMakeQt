#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class MainWindow : public QOpenGLWindow, protected QOpenGLFunctions
{
	Q_OBJECT
protected:
	QOpenGLContext context;

public:
	MainWindow();
	void paintGL() override;
};