# **[Project]()**

## *Cloning the repository*

You can clone the repository as follows:

```console
git clone "https://github.com/Fogotcheck/NIIET.git"
```

## *Project structure*

```
├── exm                                 // AppMain.cpp
|
├── Cmake
|       ├── toolchain                       // minimal set of build rules
|       ├── opts                            // additional build functions
|       └── utils                           // extra environment setup utilities
|
|
├── hal                                     // project libraries
|       ├── ...
|       └── CMakeLists.txt                  // CMakeLists for building libraries
|
|
├── CMakeLists.txt                          // main project build file
|
└── README.md                               // You are here

```

## *Project build*

### Project build setup
The project build is based on [CMake](https://cmake.org/). The minimum requirement for configuring the project build is specifying the toolchain:

```console
CMAKE_TOOLCHAIN_FILE=./Cmake/toolchain/toolchain.cmake
```

### Additional project setup utilities

[pre-commit](https://pre-commit.com) setup utility

```console
cmake --build ./build --target UTILS_examp_VIRTUAL_ENV
```

### Building the project

Build is performed with the command:

```console
cmake --build ./build
```

## *Additional links*

* [Issue tracker](https://github.com/Fogotcheck/NIIET/issues/new/choose)
