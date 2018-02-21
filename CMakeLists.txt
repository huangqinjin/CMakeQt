cmake_minimum_required(VERSION 3.9)
project(CMakeQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(ENV{Qt5_DIR} "C:\\Qt\\5.10.1\\msvc2017_64\\lib\\cmake")
# find_package(Qt5 COMPONENTS Widgets NO_CMAKE_PATH)
find_package(Qt5 COMPONENTS Widgets Quick)

foreach(t Qt5::Widgets Qt5::Quick Qt5::qmake Qt5::uic Qt5::rcc)
    get_target_property(LOC ${t} LOCATION)
    message(STATUS "***${LOC}***")
endforeach()

find_package(Eigen3 REQUIRED)

set(SOURCE_FILES main.cpp MainWindow.cpp qml.qrc)
file(GLOB HEADER_FILES *.h *.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Quick Eigen3::Eigen)
#set_property(TARGET ${PROJECT_NAME} PROPERTY Qt5_NO_LINK_QTMAIN ON)


if("${CMAKE_TOOLCHAIN_FILE}" MATCHES ".*vcpkg\.cmake$")
	get_target_property(LOC Qt5::qmake LOCATION)
	get_filename_component(LOC ${LOC} DIRECTORY)
	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		set(LOC "${LOC}/qt_debug.conf")
	else()
		set(LOC "${LOC}/qt_release.conf")
	endif()
	configure_file("${LOC}" "${PROJECT_BINARY_DIR}/qt.conf")
endif()