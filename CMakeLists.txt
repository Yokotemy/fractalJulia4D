cmake_minimum_required(VERSION 3.20)
project(fTester LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_definitions(GLM_FORCE_CUDA)

add_compile_options($<$<COMPILE_LANGUAGE:CUDA>:--expt-relaxed-constexpr>)

# --- WYSZUKIWANIE PAKIETÓW ---
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLEW REQUIRED glew)

include_directories(
        ${GLEW_INCLUDE_DIRS}
)


# --- DEFINICJA EXECUTABLE ---
add_executable(fTester
        src/main.cpp
        src/fractal/fractalParams.h
        src/app.cpp
        src/app.h
        src/utils/timer.cpp
        src/utils/timer.h
        src/UIBridge/UIBridge.cpp
        src/UIBridge/UIBridge.h
        src/renders/parallelRenderer.cu
        src/renders/parallelRenderer.cuh
        src/renders/sequentialRenderer.cpp
        src/renders/sequentialRenderer.h
        src/renders/IRenderer.h
        src/fractal/commonMath.cuh
        src/camera/camera.cpp
        src/camera/camera.h
        src/UIBridge/FractalItem.cpp
        src/UIBridge/FractalItem.h
)


find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
find_package(CUDAToolkit REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)
target_include_directories(fTester PRIVATE src)

qt_add_qml_module(fTester
        URI qml
        VERSION 1.0
        QML_FILES
        qml/Main.qml
)

# --- LINKOWANIE BIBLIOTEK ---
target_link_libraries(fTester
        PRIVATE
        GLEW                 # zamiast ${GLEW_LIBRARIES} - unika GLEWd
        glfw
        OpenGL::GL
        CUDA::cudart
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
)