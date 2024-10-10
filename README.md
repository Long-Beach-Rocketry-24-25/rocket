# rocket

(will add more later...)

Welcome to our CMake build system for STM32!

## Installing Prerequisites
First, install the arm toolchain. If you are on a Unix system, simply use your package installer, for example in Ubuntu: ```sudo apt-get install arm-none-eabi-gcc```.
On Windows, you can download it from online, try [here](https://developer.arm.com/downloads/-/gnu-rm) - you may have to add it to your path. Make sure it worked: ```arm-none-eabi-gcc --version```


Next, install Cmake. ```sudo apt-get install cmake```
On Windows, try [here](https://cmake.org/download/). Again, may have to add to path.
Make sure it worked: ```cmake --version```


Lastly, pull in external dependencies:
```git submodule update --init --recursive```


## Building
To build:
```./make.ps1 -t <name of preset>```
for example,
```./make.ps1 -t cli_h5``` (see CMakePresets.json)

for a clean build, do
```./make.ps1 -t <name of preset> -c```
