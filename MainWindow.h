#include <QOpenGLWindow>
#include <QOpenGLFunctions>

class MainWindow : public QOpenGLWindow,
				   protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MainWindow();
	
protected:
    void initializeGL() override;
    void paintGL() override;
};