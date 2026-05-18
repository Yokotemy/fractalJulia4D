cmake_minimum_required(VERSION 3.20)
project(fTester LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(fTester src/main.cpp src/fractal/fractalParams.h
        src/app.cpp
        src/app.h
        src/utils/fileUtils.cpp
        src/utils/fileUtils.h
        src/utils/timer.cpp
        src/utils/timer.h
        src/UIBridge/UIBridge.cpp
        src/UIBridge/UIBridge.h
        src/renders/dynamicRendererGLSL.cpp
        src/renders/dynamicRendererGLSL.h
        src/renders/parallelRendererCUDA.cpp
        src/renders/parallelRendererCUDA.h
        src/renders/sequentialRendererCPU.cpp
        src/renders/sequentialRendererCPU.h
        src/renders/IRenderer.h
        src/fractal/commonMath.h
        src/camera/camera.cpp
        src/camera/camera.h)

target_link_libraries(fTester
        PRIVATE
        GLEW::GLEW
        glfw
        glm::glm
        OpenGL::GL
)
