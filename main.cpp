#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLibraryInfo>
#include <QDebug>
#include <QQuickWindow>
#include "MainWindow.h"
#include "QuickRectangle.h"

void info()
{
#if QT_VERSION_MAJOR < 6
#define path location
#endif

    qDebug() << "QLibraryInfo::PrefixPath            " << QLibraryInfo::path(QLibraryInfo::PrefixPath);
    qDebug() << "QLibraryInfo::DocumentationPath	 " << QLibraryInfo::path(QLibraryInfo::DocumentationPath);
    qDebug() << "QLibraryInfo::HeadersPath	         " << QLibraryInfo::path(QLibraryInfo::HeadersPath);
    qDebug() << "QLibraryInfo::LibrariesPath	     " << QLibraryInfo::path(QLibraryInfo::LibrariesPath);
    qDebug() << "QLibraryInfo::LibraryExecutablesPath" << QLibraryInfo::path(QLibraryInfo::LibraryExecutablesPath);
    qDebug() << "QLibraryInfo::BinariesPath	         " << QLibraryInfo::path(QLibraryInfo::BinariesPath);
    qDebug() << "QLibraryInfo::PluginsPath	         " << QLibraryInfo::path(QLibraryInfo::PluginsPath);
#if QT_VERSION_MAJOR < 6
    qDebug() << "QLibraryInfo::ImportsPath           " << QLibraryInfo::path(QLibraryInfo::ImportsPath);
    qDebug() << "QLibraryInfo::Qml2ImportsPath	     " << QLibraryInfo::path(QLibraryInfo::Qml2ImportsPath);
#else
    qDebug() << "QLibraryInfo::QmlImportsPath	     " << QLibraryInfo::path(QLibraryInfo::QmlImportsPath);
#endif
    qDebug() << "QLibraryInfo::ArchDataPath          " << QLibraryInfo::path(QLibraryInfo::ArchDataPath);
    qDebug() << "QLibraryInfo::DataPath              " << QLibraryInfo::path(QLibraryInfo::DataPath);
    qDebug() << "QLibraryInfo::TranslationsPath      " << QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    qDebug() << "QLibraryInfo::ExamplesPath	         " << QLibraryInfo::path(QLibraryInfo::ExamplesPath);
    qDebug() << "QLibraryInfo::TestsPath             " << QLibraryInfo::path(QLibraryInfo::TestsPath);
    qDebug() << "QLibraryInfo::SettingsPath          " << QLibraryInfo::path(QLibraryInfo::SettingsPath);

#undef path
}

int widget(int argc, char* argv[]) 
{
    QApplication app(argc, argv);

    info();

    MainWindow w;
    w.show();

    return app.exec();
}

int quick(int argc, char* argv[])
{
#if QT_VERSION_MAJOR < 6 // High-DPI scaling is always enabled since Qt6
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    info();

    qmlRegisterType<QuickRectangle>("HelloQuick", 1, 0, "Rectangle");	

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    QQuickWindow* const w = [&engine] {
        const auto& roots = engine.rootObjects();
        return roots.size() != 1 ? nullptr : qobject_cast<QQuickWindow*>(roots.front());
    }();

    if (!w) return -1;

    w->connect(w, &QQuickWindow::beforeRendering, w, [] {
        qDebug() << "before rendering";
    }, Qt::DirectConnection);
    w->connect(w, &QQuickWindow::afterRendering, w, [] {
        qDebug() << "after rendering";
    }, Qt::DirectConnection);

    return app.exec();
}

int main(int argc, char* argv[])
{
    if (argc > 1) return widget(argc, argv);
    else return quick(argc, argv);
}
