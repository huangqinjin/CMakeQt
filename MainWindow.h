#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Compatibility>

class MainWindow : public QOpenGLWindow,
				   protected QOpenGLFunctions_3_3_Compatibility
{
	Q_OBJECT

public:
	MainWindow();
	
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};