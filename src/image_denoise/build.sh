# Without cmake
#  export LD_LIBRARY_PATH=/mnt/data/PROJECT/Workspace/openimageio/dist/lib64
#  g++ -I/mnt/data/PROJECT/Workspace/openimageio/dist/include -l /mnt/data/PROJECT/Workspace/openimageio/dist/lib64 -l OpenImageIO -l OpenImageIO_Util  -o test main.cpp 

export cxxopts_DIR=$REPO_ROOT/ext/cxxopts/dist/lib64/cmake/cxxopts
export OpenImageIO_DIR=$REPO_ROOT/ext/OpenImageIO/OpenImageIO/dist/lib64/cmake/OpenImageIO
export OpenImageDenoise_DIR=$REPO_ROOT/ext/OpenImageDenoise/OpenImageDenoise/dist/lib64/cmake/OpenImageDenoise-1.4.3
export BOOST_ROOT=/mnt/data/SOFTWARE/Boost___1.82.0___x64___linux/dist

cd $REPO_ROOT/src/image_denoise
cmake . -D CMAKE_INSTALL_PREFIX=dist -B build
cmake --build build # --clean-first              # make clean all
cmake --install build                          # make install