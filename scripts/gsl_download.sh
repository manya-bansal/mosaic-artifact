TENSOR_ALGEBRA_SRC=/home/ubuntu/tensor_algebra_systems_src
TENSOR_ALGEBRA_LIB=/home/ubuntu/tensor_algebra_systems_lib

# Step 0: Create a gsl directory where the lib will be stored. 
cd $TENSOR_ALGEBRA_LIB
mkdir gsl

# Step 1: remove the gsl tar if it exists directly 
cd $TENSOR_ALGEBRA_SRC
rm -rf gsl-2.7.1.tar.gz
rm -rf gsl-2.7.1

# Step 1: Download GSL 2.7.1
wget ftp://ftp.gnu.org/gnu/gsl/gsl-2.7.1.tar.gz
tar -zxvf gsl-2.7.1.tar.gz
rm -rf gsl-2.7.1.tar.gz

# Step 2: Build library from source and put lib + headers to in the 
# TENSOR_ALGEBRA_LIB/gsl dir.
cd gsl-2.7.1
./configure --prefix=$TENSOR_ALGEBRA_LIB/gsl && make && make install

# Step 3: Download the tensor extension
./gsl_tensor_download.sh

# Step 4: Download ATLAS (long configure)
# Uncomment the following line to run long configure atlas
# ./atlas_download.sh