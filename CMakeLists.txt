cmake_minimum_required(VERSION 3.9.2 FATAL_ERROR)

set(PROJECT_NAME PanoramaViewer)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)
message(STATUS "Requiring C++${CMAKE_CXX_STANDARD} - done")

project(${PROJECT_NAME} VERSION 0.0.0 LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

if (WIN32 AND NOT CYGWIN)
  add_definitions(-DUNICODE -D_UNICODE)
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif ()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /wd4706 /wd4996 /D_CRT_SECURE_NO_WARNINGS")
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wextra -Wswitch -Wshadow -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wformat -Wformat-security")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -Wall -Wextra -Wswitch -Wshadow -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wformat -Wformat-security -Wfloat-conversion")
endif ()

if (NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif ()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Qml Quick Core)

add_subdirectory(src)
