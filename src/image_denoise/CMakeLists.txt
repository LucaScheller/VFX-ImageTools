cmake_minimum_required(VERSION 3.12)

project(ImageToolsDenoise VERSION 1.0)

### CPP Settings ###
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin")
set(PYTHON_PACKAGE_RELATIVE_PATH lib/python)
set(INSTALL_PYTHONPACKAGE_DIR ${CMAKE_INSTALL_PREFIX}/${PYTHON_PACKAGE_RELATIVE_PATH})
set(BUILD_SHARED_LIBS ON)

### Packages ###
# set(CMAKE_BUILD_RPATH_USE_ORIGIN True)
# ArgParser cxxopts
find_package(cxxopts REQUIRED)
set(CXXOPTS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/../../ext/cxxopts/dist/include)
# OpenImageIO
find_package(OpenImageIO REQUIRED)
# OpenImageDenoise
find_package(OpenImageDenoise REQUIRED)
# Boost
set(Boost_NO_SYSTEM_PATHS true)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS python numpy REQUIRED)
# Python
find_package(Python 3.9.11 REQUIRED COMPONENTS Interpreter Development)

### Targets ###
## Target Library > denoiseEngines ##
add_library(denoiseEngines SHARED denoiseEngines.cpp)
# Props
set_target_properties(denoiseEngines PROPERTIES PREFIX "")
# Libs
target_link_directories(denoiseEngines PUBLIC ${OpenImageIO_LIB_DIR})
target_link_libraries(denoiseEngines PUBLIC 
                      OpenImageIO OpenImageIO_Util OpenImageDenoise
                      Boost::python Boost::numpy ${Python_LIBRARIES})
# Headers
target_include_directories(denoiseEngines PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${CXXOPTS_INCLUDE_DIR}
                           ${OpenImageIO_INCLUDE_DIR}
                           ${Boost_INCLUDE_DIRS} ${Python_INCLUDE_DIRS})
# Install
install(TARGETS denoiseEngines DESTINATION ${INSTALL_PYTHONPACKAGE_DIR})
