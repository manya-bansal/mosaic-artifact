#!/bin/zsh
export PATH_TO_MOSAIC_ARTIFACT=/home/manya227/mosaic-artifact
#                     # Example including tblis in path.
# export INCLUDE_PATH=" -I$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_lib/ -I$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_lib/tblis/include \
# -I$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_lib/tblis/include/tblis"

# export LIB_PATH="-L$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_lib/tblis/lib \
# -Wl,-R$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_lib/tblis/lib"

# export DYNAMIC_LIB_FILES=-l:libtblis.so.0.0.0

# export INCLUDE_HEADERS="#include \"tblis.h\"
# #include \"tblis-wrappers.h\"
# "

export TENSOR_ALGEBRA_SRC=$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_src
export TENSOR_ALGEBRA_LIB=$PATH_TO_MOSAIC_ARTIFACT/tensor_algebra_systems_lib