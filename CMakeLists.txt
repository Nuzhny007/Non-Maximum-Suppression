cmake_minimum_required(VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(nms)

find_package(OpenCV REQUIRED)

unset(CMAKE_CXX_FLAGS CACHE)

find_package(OpenMP)
if (OPENMP_FOUND)
    list(APPEND CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    list(APPEND CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g" CACHE STRING COMPILE_FLAGS FORCE)

    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g -march=native --fast-math -ffast-math -mtune=native -funroll-loops -DNDEBUG -DBOOST_DISABLE_ASSERTS" CACHE STRING COMPILE_FLAGS FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG" CACHE STRING COMPILE_FLAGS FORCE)

elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4" CACHE STRING COMPILE_FLAGS FORCE)
    add_definitions(-D_USE_MATH_DEFINES -DWIN32 -D_CRT_SECURE_NO_WARNINGS)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

set(LIBS ${OpenCV_LIBS})

add_executable(${PROJECT_NAME} main.cpp nms.h)
target_link_libraries(${PROJECT_NAME} ${LIBS})
