# Charon
Charon is a simple asio based server to transmit data from nscldaq to an external program that has no knowledge of the nscldaq ringbuffer format. This allows for online analysis with programs that are not compiled against nscldaq code.

## Building Charon
Clone Charon using `git clone --recursive https://github.com/gwm17/Charon.git`. Charon uses CMake as the build system. On Linux/MacOS you can run the following commands to build Charon: 

```
mkdir build
cd build
cmake ..
make -j 4
```

Note that Charon does need to know the location of the nscldaq install. These are set in the top level CMakeLists.txt.
Charon has been tested and built within the nscldaq singularity container.

## Requirements
- nscldaq 11.3 or greater
- c++11
- CMake > 3.12
