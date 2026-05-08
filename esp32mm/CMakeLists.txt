# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32mm)

get_filename_component(SHARED_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../shared" ABSOLUTE)