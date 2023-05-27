# Repo
export REPO_ROOT=/mnt/data/PROJECT/ImageTools

# System Libs
export LD_LIBRARY_PATH="\
${REPO_ROOT}/ext/OpenImageDenoise/OpenImageDenoise/dist/lib64:\
${REPO_ROOT}/ext/OpenImageIO/OpenImageIO/dist/lib64:\
${REPO_ROOT}/ext/OpenImageIO/ext/dist/lib:\
${REPO_ROOT}/ext/OpenImageIO/ext/dist/lib64:\
/mnt/data/SOFTWARE/Boost___1.82.0___x64___linux/dist/lib:\
${LD_LIBRARY_PATH}"

# Python
export PYTHONPATH="\
${REPO_ROOT}/image_denoise/dist/lib/python:\
${REPO_ROOT}/ext/OpenImageIO/OpenImageIO/dist/lib64/python3.9/site-packages/OpenImageIO:\
${REPO_ROOT}/ext/OpenImageIO/ext/dist/lib64/python3.9/site-packages:\
${PYTHONPATH}"
source ./ext/python/bin/activate

