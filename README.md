# rocket

(will add more later...)

Welcome to our CMake build system for STM32!

## Installing Prerequisites
First, install the arm toolchain. If you are on a Unix system, simply use your package installer, for example in Ubuntu: ```sudo apt-get install arm-none-eabi-gcc```.
On Windows, you can download it from online, try [here](https://developer.arm.com/downloads/-/gnu-rm) - you may have to add it to your path. Make sure it worked: ```arm-none-eabi-gcc --version```


Next, install Cmake. ```sudo apt-get install cmake```
On Windows, try [here](https://cmake.org/download/). Again, may have to add to path.
Make sure it worked: ```cmake --version```

Also, install Ninja ```sudo apt-get install ninja-build```
On Windows, try [here](https://github.com/ninja-build/ninja/releases). Same with adding to path.
Make sure it worked: ```ninja --version```


Lastly, pull in external dependencies:
```git submodule update --init --recursive```


## Building
To build:
```./make.ps1 -t <name of preset>```
for example,
```./make.ps1 -t stm32f746``` (see CMakePresets.json)
It's also possible to specify a target application rather than building all available apps (which is the default), by using the -a parameter: ```./make.ps1 -t stm32f746 -a cli_app```. 

for a clean build, do
```./make.ps1 -t <name of preset> -c```

builds are by default done in Debug mode, but Release mode can be selected with the -r parameter: ```./make.ps1 -t stm32f746 -r```

## Debugging
To debug, make sure you have openocd installed ```sudo apt-get install openocd```
On Windows, try [here](https://openocd.org/pages/getting-openocd.html). Also may have to add to path.
There are reference launch.json files found in the repository already under .vscode.
