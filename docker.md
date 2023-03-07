## Getting Started with Docker
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
- The above command should print out a `CONTAINER_ID`. Attach to the
  dockercontainer using the command below and the `CONTAINER_ID` from the
  previous step.
  ```
  docker attach <CONTAINER_ID>
  ```

  *Note:* Do not type `exit` in the docker terminal as this will kill the container. The proper way to exit the docker is the sequence `CTRL-p, CTRL-q`.

- Download the external functions that can be run on your machine. Refer to the [Downloading External Functions](#-downloading-external-functions) section to learn more about which machines each library can be run on and how to download these functions with our provided scripts.

## Running the Artifact
Continue on to the [Top-Level Script](README.md#top-level-script) Section in the `README.md`
 
## Downloading CPU External Functions

We provide a list of external functions that can be downloaded on a CPU and which machines they are compatible with. We also provide scripts to download and build each library. We also note any performance-related quirks related to some functions (for example, the tuning time of ATLAS).

We provide a list of known issues. Please note that this list may be not comprehensive and is based on our experience. The authors are not experts on these external systems.


| Library | Known Issues | Download Instructions | Time Taken to Complete |
| ------ | ------ | ------ | ------ |
| CBLAS                   || Done in Dockerfile ||
| MKL + AVX               || Done in Dockerfile ```sudo apt-get install libmkl-dev libmkl-avx2``` ||
| TBLIS                   | Could not successfully download on Apple M1 | ```./scripts/tblis_download.sh``` | 20 Minutes|
| GSL + Tensor  Extension + (optional) ATLAS || ```./scripts/gsl_download.sh``` | 4 minutes (GSL) + 2 minutes (Tensor Extension) + (optional ~8 hours ATLAS)|

Please note that we provide the quickest build for ATLAS without any additional 
passed in using sudo apt-install. We provide an ```atlas_download.sh``` script
that can be used to link against the tuned version. To run this and download
GSL again, uncomment the ```./atlas_download.sh``` line in
```./gsl_download.sh```.

