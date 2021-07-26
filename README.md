# How to build Grail on Linux and Windows

This document describes how to build Grail on a number of different platforms. Grail requires a number of cutting edge tools, so the default ones may have to be replaced. Specifically, we need:

  1. gcc-11. We use C++-20 language features including C++17 variadic templates, C++-20 concepts
  1. CMake. We need some specific features so require 3.16 or better.
  1. Ninja. We used make, but it is slower and labor intensive. CMake + Ninja is a big improvement.
  1. Libraries
    1. GLFW to manage windows
    1. freetype to read fonts
    1. mpv for audio and video
  1. We also use vscode to edit collaboratively, though contributors may use anything as long as they don't change our formatting conventions.

Older platforms can be modified to work, but the easiest way to develop with grail is to install on Ubuntu 20.04LTS or newer, or msys2 under windows.
Instructions can be found below for various platforms.

[![CMake](https://github.com/StevensDeptECE/GrailGUI/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/StevensDeptECE/GrailGUI/actions/workflows/cmake.yml)

## Getting Set Up - Windows

1. [Install MSYS2](https://www.msys2.org/)

2. Run MSYS2: `MSYS2 MinGW 64-bit` from the Windows Start Menu.

    - Make sure you are running the correct executable or else you will have trouble with the C++ compiler not recognizing your system correctly.

3. Install dependencies

    ``` shell
    pacman -S git nano make mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw mingw-w64-x86_64-mpv mingw-w64-x86_64-youtube-dl bison flex mingw-w64-x86_64-xz 
    ```

4. Edit `~/.bashrc` to include `export GRAIL=/path/to/GRAIL` and `export PATH=$PATH:$GRAIL/bin` on the following line. `source ~/.bashrc` the first time.

## Getting Set Up - Ubuntu

### Ubuntu 18.04

1. Install gcc-11 and g++-11

    ``` shell
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt-get update
    sudo apt-get install gcc-11 g++-11
    ```

    - Preferred, but optional
        1. Set gcc-11 and g++-11 as default compiler

            ``` shell
            sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11
            sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11
            ```

        2. Confirm that the default versions of gcc and g++ are correct

            ``` shell
            sudo update-alternatives --config gcc
            sudo update-alternatives --config g++
            ```

2. If the default version of CMake is installed, purge it from `apt`

    ``` shell
    sudo apt purge --auto-remove cmake
    ```

3. Install the latest version of CMake

    ``` shell
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
    sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
    sudo apt update
    sudo apt install kitware-archive-keyring
    sudo apt update
    sudo apt install cmake
    ```

4. Install other dependencies

    ``` shell
    sudo apt install ninja-build libglfw3-dev libfreetype6-dev mpv libmpv-dev liblzma-dev flex bison pkg-config
    ```

5. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

### Ubuntu 20.04

1. Install gcc-11 and g++-11

    ``` shell
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt-get update
    sudo apt-get install gcc-11 g++-11
    ```

    - Preferred, but optional

        1. Set gcc-11 and g++-11 as default compiler

            ``` shell
            sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11
            sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11
            ```

        2. Confirm that the default versions of gcc and g++ are correct

            ``` shell
            sudo update-alternatives --config gcc
            sudo update-alternatives --config g++
            ```

2. Install dependencies

    ``` shell
    sudo apt install make cmake ninja-build libglfw3-dev libfreetype-dev mpv libmpv-dev liblzma-dev flex bison pkg-config
    ```

3. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

## Getting Set Up - Arch-Based Linux

1. Install dependencies

    ``` shell
    sudo pacman -S glfw-x11 freetype2 base-devel cmake ninja mpv youtube-dl

    ```

    - If you are using Wayland, then install `glfw-wayland` instead of `glfw-x11`. Wayland support is currently unconfirmed.
  
2. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

## Compiling

- To compile with default settings, run `./build.sh`
- For those who want to modify the default configuration and compile themselves, we use the following:

    ``` shell
    cmake -S . -Bbuild -GNinja
    cmake --build build
    ```

- For versions of Ubuntu with custom versions of gcc, but did not set the default compiler with update-alternatives:

    ``` shell
    CC=gcc-11 CXX=g++-11 cmake -S . -Bbuild -GNinja
    cmake --build build
    ```

## Running

1. Update `test/CMakeLists.txt` to compile the file you want to compile and link it to grail.
2. `cd test && testWidgets`

     - Cursors and other features might not work for other tests as they are not 100% Linux/Windows compatible yet.

## Frequent Issues

### Need to create an SSH key to push into the repository

GitHub has a great guide on how to do this! [This guide](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) will show you how to load `ssh-agent` when your terminal starts and add a new SSH key to `ssh-agent` and GitHub

### Cloned the repository using HTTPS, but now have to push to the repository

In this instance, you'll need to change the remote url that git is pointing to. This can be done with a single git command that changes which website the remote called `origin` is pointing at. Once you run this, try to push again and you should be good to go!

Git command:

``` shell
git remote set-url origin git@github.com:StevensDeptECE/GrailGUI.git
```

### Runtime Error:  `Failed to open GLFW window`

This error is most commonly seen when attempting to run Grail on a Windows virtual machine. If your use-case is different, please open an issue with the details of your environment. As far as the project team is aware, this error can be traced to a lack of OpenGL compatibility on Windows virtual machines. This means that the issues is a driver problem that cannot be resolved until there is better OpenGL support in virtual video drivers. If you need a virtual machine, but do not need Windows, a virtual machine running Ubuntu Linux should work. Otherwise, until there is sufficient OpenGL support for virtual Windows, you will have to resort to a dual boot or alternative computer to run Grail and Grail-based programs.

### Everything Else

If you have an issue not on this list, and feel it should be included in the list of frequent issues, open an issue and let us know!
