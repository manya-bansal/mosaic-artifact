# Mosaic Artifact

Repository for Mosaic artifact generation.

### Overview

The Mosaic compiler extends functionally described in the
[TACO](https://github.com/tensor-compiler/taco) compiler and is built on top of
TACO's implementation.

 - Run Experiments:
    - Getting Started Guide (5 human-minutes + ~30 compute-minutes)
    - Step-by-Step Instructions (10 human-minutes + ~70 compute-hours)
 - Validate All Results

## Getting Started Guide [5 human-minutes + ~30 compute-minutes]

**For artifact evaluation, we highly recommend reviewers to use the provided
login to our AWS instance. This machine has access to the GPU used in the paper
and we pre-built all external software libraries.**

We have provided login instructions (username, password, and host) to three AWS
instances in the bidding instructions for the artifact evaluation.
**Please do not run benchmarks in parallel. This will
introduce noise, and the graph output may vary. This includes multiple reviewers
who may be running benchmarks on the same machine, therefore, we recommend one
machine per reviewer at a time.**

If a non-reviewer would like to run this artifact using Docker, please refer to
[these instructions](DOCKER.md).

### Kick-the-Tires Test  
To ensure that the artifact is functional please run

  ```
  cd scripts/bench-scripts
  make kick-tires
  ```

This script will run all unit tests associated with the Mosaic compiler and checks our system can successfully link against all the external libraries that were used in the evaluation. Further, it runs the `MMAdd` benchmark (Figure 18) to completion and tests the benchmarking and the graph generation code.



## Step-by-Step Instructions [10 human-minutes + ~70 compute-hours]
The step-by-step instructions include two sections:
1. Running a [Top-Level Script](#top-level-script) [5 human-minutes + ~60 compute-hours]
2. Regenerating the [Stardust](#running-stardust) CSV (which is optional) [5 human-minutes + ~8 compute-hours]

### Top-Level Script [5 human-minutes + ~60 compute-hours]
To run all benchmarks for all systems mentioned in the paper, in the directory ```scripts/bench-scripts/``` run:

  ```
  make all-benchmarks
  ```

If you want to specify which benchmarks to run for a particular figure, you can use:

  ```
  make run-fig<#>
  ```

To make a specific figure (assuming the data has been generated using the previous command), run:

  ```
  make draw-fig<#>
  ```

For example, if you want to run and draw fig13, you will run ```make run-fig13
&& make draw-fig13```.
 
We provide an estimate of how long we expect each benchmark to take:

  | Figure # | Benchmark | Time Taken |
  | :------: | ----- | ----- |
  | 13 (Page 15)| GEMV | ~3 hours |
  | 14 (Page 15)| Symmetric GEMV | ~3 hours |
  | 15 (Page 15)| SpMV | ~5 hours |
  | 16 (Page 17)| SDDMM with varying sparsity | ~8 hours |
  | 17 (Page 17)| Block Sparse: 5% non-zeros | ~14 hours  |
  | 18 (Page 17)| SpMMAdd | ~30 minutes |
  | 19 (Page 17)| SDDMM with varying dim | ~8 hours |
  | 20 (Page 17)| Block Sparse: 20% non-zeros | ~12 hours |
  | 21 (Page 17)| TTV | ~30 minutes |
  | 22 (Page 18)| Compute Capability Language | ~1 minute |

**However, if you are on your local machine, and not on the AWS machine, you can only run benchmarks that are compatible for your system. In this case, you will need to specify which external functions to target. More instructions [here](docker.md).** 

### Running Stardust (Optional) [10 human-minutes + ~8 hours]
We have already provided the numbers for running the SpMV (figure 15 on page 15) and SpMMAdd (figure 18 on page 17)
kernels on the Capstan hardware using the [Stardust compiler](https://github.com/weiya711/taco-to-spatial) (orange lines) in
`mosaic-benchmarks/stardust-runs/spmv_plus2.csv`. However, we provide a script to regenerate this CSV
from the Capstan cycle-accurate simulator
[tungsten](https://github.com/acrucker/tungsten). To regenerate the CSV,
follow the following steps.

Again in the directory ```scripts/bench-scripts/``` run:
```
make stardust-csv
```

Then rerun the following commands to make and draw figures 15 and 18: 
```
make run-fig15 && make draw-fig15
```
```
make run-fig18 && make draw-fig18
```

### Validate Results

To move the figures over to your local machine for viewing, please run:

  ```
    scp -r <username>@<host>:/home/reviewer/scripts/bench-scripts/figs <path_on_local_machine>
  ```

  - Validate that `fig13.pdf` matches the trends in Figure 13 on page 15.
  - Validate that `fig14.pdf` matches the trends in Figure 14 on page 15.
  - Validate that `fig15.pdf` matches the trends in Figure 15 on page 15.
  - Validate that `fig16.pdf` matches the trends in Figure 16 on page 17.
  - Validate that `fig17.pdf` matches the trends in Figure 17 on page 17.
  - Validate that `fig18.pdf` matches the trends in Figure 18 on page 17.
  - Validate that `fig19.pdf` matches the trends in Figure 19 on page 17.
  - Validate that `fig20.pdf` matches the trends in Figure 20 on page 17.
  - Validate that `fig21.pdf` matches the trends in Figure 21 on page 17.
  - Validate that the output of `make run-fig22` matches Table 22. Please note since this benchmark performs a random search, one can expect to see a variance of up to 10 seconds for long running searches. We are running on a new machine, so we expect to see some variance (due to machine noise and machine state) in the runtimes of each figure (13-21), but the trends should still match.

 Note for `fig18.pdf`: While preparing the artifact, we discovered a small bug in our code which has been brought to the Artifact Evaluation Chairs. The graph that is produced draws both lines, the buggy implementation and the corrected version. 

## Reusing the Artifact Beyond the Paper 

Please note that all active development beyond this paper is located in the [mosaic](https://github.com/manya-bansal/mosaic) repository and not the [mosaic-artifact](https://github.com/manya-bansal/mosaic-artifact) (this) repository. The mosaic repository is already included as a submodule within this repository.

### Adding New External Functions

Each external function is included like a library with a ```.h``` file. To add external functions to Mosaic, users need to define a class that provides both the imperative algorithm for code generation and the semantics of the function. Example headers are implemented in the ```mosaic/include/taco/accelerator_interface``` directory.

To demonstrate how to plug-in new functions to Mosaic, we walk through the
process of adding new external functions. To make our discussion concrete, we
consider an example of the
[CBLAS](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines.html)
library, in particular the
[```cblas_saxpy```](https://www.intel.com/content/www/us/en/develop/documentation/onemkl-developer-reference-c/top/blas-and-sparse-blas-routines/blas-routines/blas-level-1-routines-and-functions/cblas-axpy.html#cblas-axpy)
function. The ```cblas_saxpy``` function computes the sum of a vector-scalar
product and another vector, and has the interface:

```void cblas_saxpy (const MKL_INT n, const float a, const float *x, const MKL_INT incx, float *y, const MKL_INT incy);```


For simplicity, we only consider the case where our scalar is 1 i.e.
```cblas_saxpy``` computes the sum of two vectors. In einsum notation, the
semantics of the ```cblas_saxpy``` are given by: ```Y(i) = X(i) + Y(i)```.

The arguments to the ```cblas_saxpy``` function are given by:

1. ```const MKL_INT n```: Length of ```X```, which must be equal to ```Y```.
2. ```const float a```: Scalar to multiply  ```X```, in our case 1.
3. ```const float *x```: Pointer to storage of ```X```.
4. ```const MKL_INT incx```: Stride to access next element of ```X```.
5. ```const float *y```: Pointer to storage of ```Y```.
6. ```const MKL_INT incy```: Stride to access next element of ```Y```.

Now, to define the external function interface (as defined in Section 3), first
we must first define a class that inherits from the pure virtual
```AbstractFunctionInterface``` class defined in
```mosaic/include/taco/accelerator_notation/accel_interface.h```. We elide some details to keep our
discussion short, but to look at the complete definition, refer to the
```Saxpy``` class defined in
```mosaic/include/taco/accelerator_interface/cblas_interface.h```.

First, we define the semantic description of the functions:

```
    taco::AcceleratorStmt getStmt() const override{ return x(i) = x(i) + y(i);}
```
Here, ```x``` and ```y``` are of private variables of type ```TensorObject```.
The ```TensorObject``` extends TACO tensors to support dynamic orders. To see
an example of writing language capability statements that use dynamic tensors,
refer to ```mosaic/test/test-accelerate-notation.cpp```.

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
We use special objects like ```DimArg```, ```LiteralArg```,
```TensorObjectArg``` to pass in tensor metadata and literal arguments. More
types of arguments are defined in
```mosaic/include/taco/accelerator_notation/accel_interface.h```.

Finally, we define the return type and function name:

```
    std::string getReturnType() const override {return "void";}
    std::string getFunctionName() const override{return "cblas_saxpy";}
```
Note that we also define a pass through checker function since we do not need to specify any other constraint's on the ```cblas_saxpy``` function.

```
bool checkerFunction(IndexStmt stmt) const override{return true;}
```

*To see a more complicated example, refer to the ```tblis_interface.h```*.
Here, one can note the ```callBefore``` and ```callAfter``` functionality in
action. One can also see how library-specific objects can be used as arguments
through the use of ```DeclVar```.

### Scheduling a Call to cblas\_saxpy

To ```map``` or  ```bind``` a call to the ```Saxpy``` functions, use the
```accelerate``` (aliased) scheduling command. Note that the ```accelerate```
command is overloaded to provide the functionality of both the ```bind``` and
```map``` command. The ```bind``` functionality is implicitly included because
we do not overwrite previously applied scheduling command.

To see examples of using this command, refer to
```mosiac/test/test-interface.cpp```. A call to ```Saxpy``` has been scheduled
at `line 132` of the test.

To schedule a call using the automatic mapper, fist call the
```registerAccelerator``` function with a ```Saxpy```  object passed in as an
argument. Next, call ```accelerateOn``` command that chooses a schedule to
apply. Because our paper does not select best mapping i.e. we do not auto-tune
our mappings, we automatically apply the first schedule.

### Exploring the Code

Here, we provide pointers to places in the code that implement key
functionality:

1. External Function Interface: ```mosaic/include/taco/accelerator_interface```.
2. Code Generation to target the Z3 theorem prover:
   ```mosaic/include/taco/accelerator_notation/code_gen_dynamic_order.h``` and the corresponding
   implementation in ```mosaic/src/accelerator_notation/code_gen_dynamic_order.cpp```
3. Defintion of the function capability language, aliased as ```DynamicStmt```
   in the code: ```mosaic/include/taco/accelerator_notation/accelerator_notation.h``` and
   corresponding implementation in ```mosaic/src/accelerator_notation/accelerator_notation.cpp```.
4. Key search generation and checking:
   ```mosaic/include/taco/accelerator_notation/accelerate_search.h``` and the corresponding
   implementation in ```mosaic/src/accelerator_notation/accelerate_search.cpp```. 
   There are also additional
   mathematical rewrite functions in ```mosaic/src/index_notation/index_notation.cpp```.
5. Scheduling commands: ```mosaic/include/taco/index_notation/index_notation.h``` and the
   corresponding implementation in ```mosaic/src/index_notation/index_notation.cpp```. 
