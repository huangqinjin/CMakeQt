cmake_minimum_required(VERSION 3.9)
project(CMakeQt)

set(CMAKE_CXX_STANDARD 17)
# set(ENV{Qt5_DIR} "C:\\Qt\\5.10.1\\msvc2017_64\\lib\\cmake")
# find_package(Qt5 COMPONENTS Core Widgets Gui NO_CMAKE_PATH)
find_package(Qt5 COMPONENTS Core Widgets Gui)

foreach(t Qt5::Core Qt5::Widgets Qt5::Gui)
    get_property(LOC TARGET ${t} PROPERTY IMPORT_LOCATION)
    message(STATUS "***${LOC}***")
endforeach()


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Widgets Qt5::Gui)
