cmake_minimum_required(VERSION 3.12)
project(CMakeQt)

set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO RelWithDebInfo Release "")
set(CMAKE_MAP_IMPORTED_CONFIG_RELEASE Release RelWithDebInfo "")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_FIND_DEBUG_MODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#set(QT_VERSION_MAJOR 5)
# https://doc.qt.io/qt-6/cmake-qt5-and-qt6-compatibility.html
if (NOT DEFINED QT_VERSION_MAJOR)
   set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
   set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
   find_package(QT 5.15 REQUIRED NAMES Qt6 Qt5 COMPONENTS Core)
else()
    find_package(Qt${QT_VERSION_MAJOR} 5.15 REQUIRED COMPONENTS Core)
endif()
message(STATUS "QT_VERSION_MAJOR: ${QT_VERSION_MAJOR}")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(Qt5_ROOT "C:/Qt/5.15.2/msvc2019_64")
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Quick)
if (QT_VERSION_MAJOR LESS 6)
    add_library(Qt${QT_VERSION_MAJOR}::OpenGLWidgets ALIAS Qt${QT_VERSION_MAJOR}::Widgets)
    add_library(Qt::OpenGLWidgets ALIAS Qt${QT_VERSION_MAJOR}::Widgets)
else()
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS OpenGLWidgets)
endif()

foreach(t IN ITEMS qmake uic rcc Widgets OpenGLWidgets Quick)
    get_target_property(LOC Qt${QT_VERSION_MAJOR}::${t} LOCATION)
    message(STATUS "Qt${QT_VERSION_MAJOR}::${t} ${LOC}")
endforeach()

find_package(Eigen3 REQUIRED)

set(SOURCE_FILES main.cpp MainWindow.cpp QuickRectangle.cpp)
set(RESOURCE_FILES qml.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCE_FILES})
# target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_DEPRECATED)
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen Qt::OpenGLWidgets Qt::Quick)
#set_property(TARGET ${PROJECT_NAME} PROPERTY Qt5_NO_LINK_QTMAIN ON)


enable_testing()
add_test(NAME "QtQuick" COMMAND ${PROJECT_NAME})
add_test(NAME "QtWidget" COMMAND ${PROJECT_NAME} widget)
