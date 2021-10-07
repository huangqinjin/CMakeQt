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
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(Qt5_ROOT "C:/Qt/5.15.2/msvc2019_64")
find_package(Qt5 COMPONENTS Widgets Quick QuickControls2)

foreach(t Qt5::Widgets Qt5::Quick Qt5::QuickControls2 Qt5::qmake Qt5::uic Qt5::rcc)
    get_target_property(LOC ${t} LOCATION)
    message(STATUS "${t} ${LOC}")
endforeach()

find_package(Eigen3 REQUIRED)

set(SOURCE_FILES main.cpp MainWindow.cpp QuickRectangle.cpp)
set(RESOURCE_FILES qml.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen Qt5::Widgets Qt5::Quick Qt5::QuickControls2)
#set_property(TARGET ${PROJECT_NAME} PROPERTY Qt5_NO_LINK_QTMAIN ON)


enable_testing()
add_test(NAME "QtQuick" COMMAND ${PROJECT_NAME})
add_test(NAME "QtWidget" COMMAND ${PROJECT_NAME} widget)
