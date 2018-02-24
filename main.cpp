#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLibraryInfo>
#include <QDebug>
#include <QQuickWindow>
#include "MainWindow.h"
#include "QuickRectangle.h"

int widget(int argc, char* argv[]) 
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}

int quick(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    qDebug() << "QLibraryInfo::PrefixPath            " << QLibraryInfo::location(QLibraryInfo::PrefixPath);
    qDebug() << "QLibraryInfo::DocumentationPath	 " << QLibraryInfo::location(QLibraryInfo::DocumentationPath);
    qDebug() << "QLibraryInfo::HeadersPath	         " << QLibraryInfo::location(QLibraryInfo::HeadersPath);
    qDebug() << "QLibraryInfo::LibrariesPath	     " << QLibraryInfo::location(QLibraryInfo::LibrariesPath);
    qDebug() << "QLibraryInfo::LibraryExecutablesPath" << QLibraryInfo::location(QLibraryInfo::LibraryExecutablesPath);
    qDebug() << "QLibraryInfo::BinariesPath	         " << QLibraryInfo::location(QLibraryInfo::BinariesPath);
    qDebug() << "QLibraryInfo::PluginsPath	         " << QLibraryInfo::location(QLibraryInfo::PluginsPath);
    qDebug() << "QLibraryInfo::ImportsPath           " << QLibraryInfo::location(QLibraryInfo::ImportsPath);
    qDebug() << "QLibraryInfo::Qml2ImportsPath	     " << QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath);
    qDebug() << "QLibraryInfo::ArchDataPath          " << QLibraryInfo::location(QLibraryInfo::ArchDataPath);
    qDebug() << "QLibraryInfo::DataPath              " << QLibraryInfo::location(QLibraryInfo::DataPath);
    qDebug() << "QLibraryInfo::TranslationsPath      " << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    qDebug() << "QLibraryInfo::ExamplesPath	         " << QLibraryInfo::location(QLibraryInfo::ExamplesPath);
    qDebug() << "QLibraryInfo::TestsPath             " << QLibraryInfo::location(QLibraryInfo::TestsPath);
    qDebug() << "QLibraryInfo::SettingsPath          " << QLibraryInfo::location(QLibraryInfo::SettingsPath);

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
