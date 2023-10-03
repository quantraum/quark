![Quark - Github Banner](https://github.com/quantraum/quark/assets/91737697/b669376f-8de9-4b98-a6b2-341361060326)

# Quark
## Abstract
Quark is a poorly named implementation of a Perceptron in C++.
Primarily a C++ implementation, its goal is also to be built as a Python module.
It also contains a fully featured jupyter notebook that goes trough various datasets and presents result as form as graphics with matplotlib.

## Norms
* **C++** - C++20
* **Python** - Python 3.9

## Building
Quark uses [xmake](https://xmake.io/#/) to build the C++ library and Python module.
Since xmake pulls and builds dependencies automatically, you don't need to worry about that, but an Internet connection is required.

### Building
You can build the library, Python module, sample applications and test suite with xmake: 
Run the `xmake` command in the root directory and everything should be fine.

You might want to generate a CMake configuration for some reason (using CLion as an IDE for example...). 
You may generate a CMakeLists.txt using the `xmake project -k cmake` command which will also handle all dependencies for you.

### Python module specifics
The build of the Python module implies some aspects that may be of some interest to you:
- Since Python module toolchain only supports C++ syntax up to C++ 14 : The library source (.cpp files) may contain C++ 20 syntax, but the *included* header files (.h) cannot.
You may use modern syntax in .h files that are not included in the Python module.
- The build of the module is normally done running `xmake` in the root directory as well. However, it may appear that your "pip" file is not found.
If this happens, a simple re-run of the command usually fixes this.
- If your changes don't reflect in your python environment, it might be useful to proceed with the following steps:
  - Run `xmake c` command to clean the project
  - Delete the *build* directory
  - Run `xmake` command to build from scratch

## Running
Several modules are built into this project:
* **Quark** - The C++ library
* **PyQuark** - The Python module
* **QuarkTest** - A test program for the C++ library
* **QuarkSample** - A sample program for the C++ library

Besides the C++ based modules, you also have extra directories :
* **PythonTest** - Contains Python code that uses the module
* **QuarkNotebook** - Contains a Jupyter Notebook using and demonstrating Quark 
