#!/bin/bash
# Step 0: Create a tensor directory where the lib will be stored. 
cd $TENSOR_ALGEBRA_LIB
mkdir tensor

# Step 1: remove the tensor directory if it exists directly 
cd $TENSOR_ALGEBRA_SRC
rm -rf tensor

# Step 1: Download
git clone https://github.com/zhtvk/tensor.git

# Step 2: Build library from source and put lib + headers to in the 
# TENSOR_ALGEBRA_LIB/tensor dir.

cd tensor
mkdir build
cd build
../configure --prefix=$TENSOR_ALGEBRA_LIB/tensor
make -j
make install