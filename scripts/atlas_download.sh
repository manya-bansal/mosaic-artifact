TENSOR_ALGEBRA_SRC=/home/ubuntu/tensor_algebra_systems_src
TENSOR_ALGEBRA_LIB=/home/ubuntu/tensor_algebra_systems_lib

# Step 0: Create an atlas directory where the lib will be stored. 
cd $TENSOR_ALGEBRA_LIB
mkdir atlas

# Step 1: remove the atlas tar if it exists directly 
cd $TENSOR_ALGEBRA_SRC
rm -rf atlas3.10.3.tar.bz2
rm -rf atlas

# Step 1: Download ATLAS 10.3
wget https://sourceforge.net/projects/math-atlas/files/Stable/3.10.3/atlas3.10.3.tar.bz2
bunzip2 -c atlas3.10.3.tar.bz2  | tar xfm -
mv ATLAS atlas
rm -rf atlas3.10.3.tar.bz2

# Step 2: Build library from source and put lib + headers to in the 
# TENSOR_ALGEBRA_LIB/atlas dir.
cd atlas
mkdir build
cd build
../configure --prefix=$TENSOR_ALGEBRA_LIB/atlas && make && make install