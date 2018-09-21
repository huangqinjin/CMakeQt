cmake_minimum_required(VERSION 3.9)
project(CMakeQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set(ENV{Qt5_DIR} "C:\\Qt\\5.10.1\\msvc2017_64\\lib\\cmake")
# find_package(Qt5 COMPONENTS Widgets NO_CMAKE_PATH)
find_package(Qt5 COMPONENTS Widgets Quick QuickControls2)

foreach(t Qt5::Widgets Qt5::Quick Qt5::QuickControls2 Qt5::qmake Qt5::uic Qt5::rcc)
    get_target_property(LOC ${t} LOCATION)
    message(STATUS "***${LOC}***")
endforeach()

find_package(Eigen3 REQUIRED)

set(SOURCE_FILES main.cpp MainWindow.cpp QuickRectangle.cpp)
file(GLOB HEADER_FILES *.h *.hpp)
set(RESOURCE_FILES qml.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen Qt5::Widgets Qt5::Quick Qt5::QuickControls2)
#set_property(TARGET ${PROJECT_NAME} PROPERTY Qt5_NO_LINK_QTMAIN ON)


if("${CMAKE_TOOLCHAIN_FILE}" MATCHES ".*vcpkg\.cmake$")

	if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
		get_target_property(LOC Qt5::QuickControls2 IMPORTED_LOCATION_DEBUG)
	else()
		get_target_property(LOC Qt5::QuickControls2 IMPORTED_LOCATION_RELEASE)
	endif()
	file(COPY "${LOC}" DESTINATION "${PROJECT_BINARY_DIR}")
	string(REPLACE "QuickControls2" "QuickTemplates2" LOC "${LOC}")
	file(COPY "${LOC}" DESTINATION "${PROJECT_BINARY_DIR}")
endif()
