# local-file-server

A simple HTTP File server written in C++17.

Designed for sharing files across a local network (or with VMs). 

 * Cross-platform (Windows / linux).
 * Directory listings


## Building

### Building on Windows with Visual Studio

There is a solution file (.sln) in the top level of the repo.  You will need Visual Studio 2017 or later. 

### Building on Linux with CMake

Open a console at the top level of the repo.  Then type:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
