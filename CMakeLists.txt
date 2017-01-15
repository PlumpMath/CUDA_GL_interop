cmake_minimum_required(VERSION 2.8.11)

project(testproject)

# Set Qt path
set(CMAKE_PREFIX_PATH "/Users/dateng/Qt/5.7/clang_64")
set(CMAKE_CXX_COMPILER "/Users/dateng/Developer/apple_legacy/Xcode_7.2.xctoolchain/usr/bin/c++")
set(CMAKE_C_COMPILER "/Users/dateng/Developer/apple_legacy/Xcode_7.2.xctoolchain/usr/bin/cc")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.

# CUDA library
find_package(CUDA QUIET REQUIRED)
include_directories( "/Developer/NVIDIA/CUDA-8.0/include" )

# OpenGL & GLEW library
find_package(OpenGL)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(QT5OpenGL)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


cuda_add_library( cudaLib  cudaGLInteropHelper.cu CudaHelper.cu )

# Tell CMake to create the glcudainterop executable
add_executable(glcudainterop main.cpp mainwindow.cpp GLWidget.cpp )

# Use the Widgets module from Qt 5.
qt5_use_modules(glcudainterop Widgets OpenGL)

target_link_libraries( glcudainterop cudaLib )

target_link_libraries(glcudainterop ${QT_LIBRARIES} ${OPENGL_LIBRARIES})