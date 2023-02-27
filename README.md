## Mosaic Artifact

Repository for Mosaic artifact generation.

## Overview

The Mosaic compiler extends functionally described in the [TACO](https://github.com/tensor-compiler/taco) compiler and is built on top of TACO's implementation.

## Getting Started

## Top-Level Script

## Run All Benchmarks

## Validate Results

## [Optional] Reusing the Artifact

Please note that all active development beyond this paper is located in the [mosaic](https://github.com/manya-bansal/mosaic) repository and not the mosaic-artifact (this) repository. The mosaic repository is already included as a submodule within this repository.

### Adding new external functions.

To add additional external functions to Mosaic, users need to define a header file in ```mosaic/include/taco/accelerator_interface``` directory. To make the process of adding new external functions concrete, we walk through an example of the [CBLAS](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines.html) interface.

Consider the [```cblas_saxpy```](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines/blas-routines/blas-level-1-routines-and-functions/cblas-axpy.html#cblas-axpy) function. The ```cblas_saxpy``` function computes the sum of a vector-scalar product and another vector, and has the interface:

```void cblas_saxpy (const MKL_INT n, const float a, const float *x, const MKL_INT incx, float *y, const MKL_INT incy);```


For simplicity, we only consider the case where our scaler is 1 i.e. ```cblas_saxpy``` computes the sum of two vectors. In einsum notation, the semantics of the ```cblas_saxpy``` are given by: ```Y(i) = X(i) + Y(i)```.

The arguments to the ```cblas_saxpy``` function are given by:

1. ```const MKL_INT n```: Length of ```X```, which must be equal to ```Y```.
2. ```const float a```: Scalar to multiply  ```X```, in our case 1.
3. ```const float *x```: Pointer to storage of ```X```.
4. ```const MKL_INT incx```: Stride to access next element of ```X```.
5. ```const float *y```: Pointer to storage of ```Y```.
6. ```const MKL_INT incy```: Stride to access next element of ```Y```.

Now, to define the interface, first we must inherit form a

### Scheduling a call to cblas_saxpy.

### Calling the Automatic Mapper.

### Targeting other functions.






