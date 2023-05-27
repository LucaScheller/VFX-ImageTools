# System dependencies
dnf install boost boost-devel tbb tbb-devel
dnf install python3.9

# Define repo root
export REPO_ROOT=/mnt/data/PROJECT/ImageTools

# Python
python -m venv $REPO_ROOT/ext/python
source $REPO_ROOT/ext/python/bin/activate
pip install numpy

# OpenImageIO
# Download latest release https://github.com/OpenImageIO/oiio/releases > 2.4.7.0
# Software external build scripts
cd $REPO_ROOT/ext/OpenImageIO
OpenImageIO/src/build-scripts/build_openexr.bash (Upgraded to v3.1.7)
OpenImageIO/src/build-scripts/build_opencolorio.bash (Upgraded to v2.2.1 and -DOCIO_BUILD_PYTHON=ON -DOCIO_PYTHON_VERSION=3.9.16)
OpenImageIO/src/build-scripts/build_zlib.bash
OpenImageIO/src/build-scripts/build_pugixml.bash
OpenImageIO/src/build-scripts/build_pybind11.bash
OpenImageIO/src/build-scripts/build_libtiff.bash
OpenImageIO/src/build-scripts/build_libpng.bash
OpenImageIO/src/build-scripts/build_gif.bash
OpenImageIO/src/build-scripts/build_OpenJPEG.bash
OpenImageIO/src/build-scripts/build_libjpeg-turbo.bash
OpenImageIO/src/build-scripts/build_webp.bash
OpenImageIO/src/build-scripts/build_libraw.bash
# Open CMake GUI
# Set main dependency root
OIIO_LOCAL_DEPS_PATH=$REPO_ROOT/ext/OpenImageIO/ext/dist
ZLIB_LIBRARY_RELEASE=$REPO_ROOT/ext/OpenImageIO/ext/dist/lib/libz.so
# Set other dependency roots:
# Set /mnt/data/PROJECT/ImageTools/image_multipart_converter/OpenImageIO/ext/dist
# for OpenEXR_ROOT, pybind11_ROOT, PNG_ROOT, WebP_ROOT, OpenJPEG_ROOT, JPEGTurbo_ROOT,
# GIF_ROOT, OpenColorIO_ROOT, ZLIB_ROOT, TIFF_ROOT
BUILD_MISSING_DEPS=ON
BUILD_TESTING=OFF
OIIO_BUILD_TESTS=OFF
PYTHON_VERSION=3.9.16
# Compile
cd $REPO_ROOT/ext/OpenImageIO/OpenImageIO/build
make install

# OpenImageDenoise
# Open CMake GUI
ISPC_EXECUTABLE=<PathToYouISPC-INSTALL>
CMAKE_INSTALL_PREFIX=$REPO_ROOT/ext/OpenImageDenoise/dist
# Compile
cd $REPO_ROOT/ext/OpenImageDenoise/OpenImageDenoise/build
make install

# BOOST
# Follow Boost install instructions (Python + Numpy modules)