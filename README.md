## Mosaic Artifact

Repository for Mosaic artifact generation.

## Overview

The Mosaic compiler extends functionally described in the [TACO](https://github.com/tensor-compiler/taco) compiler and is built on top of TACO's implementation.

## Getting Started

**For artifact evaluation, we highly recommend reviewers to use the provided login to our AWS instance. This machine has access to the GPU used in the paper. and we have pre-built all external software libraries. If you are logged onto the machine, please skip to the [Top-Level Script](#top-level-script) section.**

The following instructions only apply if you are not working on our AWS machine.

- Download this repository and run the following commands to build and attach a docker image from the provided Dockerfile.

```
git clone <link to this repo>
git submodule update --init --recursive
docker build -t mosaic-artifact .
```

- Once the image is built, run a docker container with a bash terminal
  ```
  docker run -d -it --rm mosaic-artifact bash
  ```
- The above command should print out a `CONTAINER_ID`. Attach to the docker   container using the command below and the `CONTAINER_ID` from the previous step.
  ```
  docker attach <CONTAINER_ID>
  ```

*Note:* Do not type `exit` in the docker terminal as this will kill the container. The proper way to exit the docker is the sequence `CTRL-p, CTRL-q`.

- Download the external functions that can be run on your machine. Refer to the[Downloading External Functions](#-downloading-external-functions) section to learn more about which machines the libraries can be run on and how to download these functions with our provided scripts.

## Top-Level Script

## Run All Benchmarks

## Validate Results

## Reusing the Artifact Beyond the Paper 

Please note that all active development beyond this paper is located in the [mosaic](https://github.com/manya-bansal/mosaic) repository and not the mosaic-artifact (this) repository. The mosaic repository is already included as a submodule within this repository.

### Adding new external functions.

Each external function is included like a library with a ```.h``` file. To add external functions to Mosaic, users need to define a class with provides both the imperitive algorithm for code generation and the semantics of the function. Example headers are implemented in the ```mosaic/include/taco/accelerator_interface``` directory.

To demonstrate how to plug-in new functions to Mosaic, we walk through the process of adding new external functions. To make our discussion concrete, we consider an example of the [CBLAS](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines.html) library, in particular the [```cblas_saxpy```](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines/blas-routines/blas-level-1-routines-and-functions/cblas-axpy.html#cblas-axpy) function. The ```cblas_saxpy``` function computes the sum of a vector-scalar product and another vector, and has the interface:

```void cblas_saxpy (const MKL_INT n, const float a, const float *x, const MKL_INT incx, float *y, const MKL_INT incy);```


For simplicity, we only consider the case where our scalar is 1 i.e. ```cblas_saxpy``` computes the sum of two vectors. In einsum notation, the semantics of the ```cblas_saxpy``` are given by: ```Y(i) = X(i) + Y(i)```.

The arguments to the ```cblas_saxpy``` function are given by:

1. ```const MKL_INT n```: Length of ```X```, which must be equal to ```Y```.
2. ```const float a```: Scalar to multiply  ```X```, in our case 1.
3. ```const float *x```: Pointer to storage of ```X```.
4. ```const MKL_INT incx```: Stride to access next element of ```X```.
5. ```const float *y```: Pointer to storage of ```Y```.
6. ```const MKL_INT incy```: Stride to access next element of ```Y```.

Now, to define the external function interface (as defined in Section 3), first we must first define a class that inherits from the pure virtual ```AbstractFunctionInterface``` class defined in ```mosaic/includes/accel-interface.h```. We elide some details to keep our discussion short, but to look at the complete definition, refer to the ```Saxpy``` class defined in ```mosaic/include/accelerator_interface/cblas_interface.h```.

First, we define the semantic description of the functions:

```
    taco::AcceleratorStmt getStmt() const override{ return x(i) = x(i) + y(i);}
```
Here, ```x``` and ```y``` are of private variables of type ```TensorObject```. The ```TensorObject``` extends TACO tensors to support dynamic orders. To see an example of writing language capability statements that use dynamic tensors, refer to ```test/tests-accelerate-notation.cpp```.

Next, we define the arguments of our function:

```
std::vector<Argument> getArguments() const override
                        {return 
                            {new DimArg(i),
                            new LiteralArg(Datatype(taco::UInt32), 1),
                            new TensorObjectArg(y),
                            new LiteralArg(Datatype(taco::UInt32), 1),
                            new TensorObjectArg(x),
                            new LiteralArg(Datatype(taco::UInt32), 1)};
                        }
```
We use special objects like ```DimArg```, ```LiteralArg```, ```TensorObjectArg``` to pass in tensor metadata and literal arguments. More types of arguments are defined in ```mosaic/include/accelerator_interface/accel_interface.h```.

Finally, we define the return type and function name:

```
    std::string getReturnType() const override {return "void";}
    std::string getFunctionName() const override{return "cblas_saxpy";}
```
Note that we also define a pass through checker function since we do not need to specify any other constraint's on the ```cblas_saxpy``` function.

```
bool checkerFunction(IndexStmt stmt) const override{return true;}
```

*To see a more complicated example, refer to the ```tblis_interface.h```*. Here, one can note the ```callBefore``` and ```callAfter``` functionality in action. One can also see how library-specific objects can be used as arguments through the use of ```DeclVar```.

### Scheduling a call to cblas_saxpy.

To ```map``` or  ```bind``` a call to the ```Saxpy``` functions, use the ```accelerate``` (aliased) scheduling command. Note that the ```accelerate``` command is overloaded to provide the functionality of both the ```bind``` and ```map``` . The ```bind``` functionality is implicitly included because we do not overwrite previously applied scheduling command.

To see examples of using this command, refer to ```test/tests-interface.cpp```. A call to ```Saxpy``` has been scheduled at `line 132` of the test.

To schedule a call using the automatic mapper, fist call the ```registerAccelerator``` function with a ```Saxpy```  object passed in as an argument. Next, call ```accelerateOn``` command that chooses a schedule to apply. Because our paper does not select best mapping i.e. we do not auto-tune our mappings, the ```accelerateOn``` function applies the first schedule.

### Exploring the code.

Here, we provide pointers to places in the code that implement key functionality:

1. External Function Interface: ```mosaic/include/taco/accel_interface```.
2. Code Generation to target the Z3 theorem prover:  ```mosaic/include/taco/code_gen_dynamic_order.h``` and the corresponding implementation in ```code_gen_dynamic_order.cpp``` located in the ```mosaic/src/accelerator_notation``` directory.
3. Defintion of the function capability language, aliased as ```DynamicStmt``` in the code: ```mosaic/include/taco/accelerator-notation.h``` and corresponding implementation in ```accelerator_notation.cpp``` located in the ```taco/mosaic/src``` directory.
4. Key search generation and checking: ```mosaic/include/taco/accelerator_search.h``` and the corresponding implementation in ```accelerator_search.cpp``` located in the ```mosaic/src/accelerator_notation``` directory. There are also additional mathematical rewrite functions in ```index_notation.cpp```.
5. Scheduling commands: ```mosaic/include/taco/index_notation.h``` and the corresponding implementation in ```index_notation.cpp``` located in the ```mosaic/src/index_notation``` directory.

## Downloading CPU External Functions

We provide a list of external functions that can be downloaded on a CPU and which machines they are compatible with. We also provide scripts to download and build each library. We also note any performance-related quirks related to some functions (for example, the tuning time of ATLAS).

Please note that this list may be not comprehensive and is based on our experience on an x86 machine and a Mac M1 machine. Please note that the authors are not experts on these systems; this list is simply intended to communicate known issues.


| Library | Compatible Machines | Download Instructions | Time Taken to Complete |
| ------ | ------ | ------ | ------ |
| CBLAS                   || sudo apt-get install libopenblas-dev ||
| MKL + AVX               || sudo apt-get install libmkl-dev libmkl-avx2||
| TBLIS                   || ./scripts/tblis_download.sh | 10 Minutes|
| GSL + Tensor  Extension + ATLAS ||  ./scripts/gsl_download.sh | 4 minutes (GSL) + 2 minutes (Tensor Extension) + (ATLAS)|

Please note that we provide the quickest build for ATLAS without any additional 
passed in. To configure ATLAS for your machine please see the [ATLAS install information](https://math-atlas.sourceforge.net/atlas_install/node6.html). For our paper, we did provide machine metadata to the configure command.

## Misc Notes for Manya: DO BEFORE SUBMITTING ARTIFACT

Set flags to:

```
set(C_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Woverloaded-virtual -pedantic-errors -Wno-deprecated")
```

Benchmark suite complains if ```-Wmissing-declarations ``` is added.

Make sure line numbers of code match if changes have been made to the Mosaic code base + double check that the locations match. 






