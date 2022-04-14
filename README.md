# How to build Grail on Linux and Windows

This document describes how to build Grail on a number of different platforms.
Grail requires a number of cutting edge tools, so the default ones may have to
be replaced. Specifically, we need:

- gcc-10, or another compiler that supports C++17 fully and has most features of
  C++20. We use C++20 language features such as the numbers header and concepts
  for templates, as well as C++17 variadic templates.
- CMake version 3.16 or later.

Libraries

- GLFW to manage windows
- freetype to read fonts
- mpv for audio and video

Other Utilities

- We use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to enforce
  formatting specifications, which are in the `.clang-format` file. In the
  future we will have an automated system that formats all code submitted to
  these specs, but in the meantime please format your code accordingly.
- VS Code uses clang-format by default when working with C++, so it's a good
  place to start off if you're looking for an editor. We also often use it to
  edit collaboratively as well.
- [Doom Emacs](https://github.com/hlissner/doom-emacs) is another popular choice
  for an editor, and is well documented for getting up and running editing C++.

If you'd like to begin working on Grail, the easiest way to do so would be to
install a copy of Ubuntu 20.04 LTS or newer, or MSYS2 if you are using Windows.
It is possible to make Linux distributions with older package versions work, but
it will require additional setup, and some parts of Grail may not work (mainly
audio and video).

[![CMake](https://github.com/StevensDeptECE/GrailGUI/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/StevensDeptECE/GrailGUI/actions/workflows/cmake.yml)

## Getting Set Up - Windows

1. [Install MSYS2](https://www.msys2.org/)

2. Run MSYS2: `MSYS2 MinGW 64-bit` from the Windows Start Menu.

   - Make sure you are running the correct executable or else you will have
     trouble with the C++ compiler not recognizing your system correctly.

3. Install dependencies

   ```shell
   pacman -S git nano make mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw mingw-w64-x86_64-mpv mingw-w64-x86_64-youtube-dl bison flex mingw-w64-x86_64-xz
   ```

<!-- This may be possible to remove in the very near future, I've nearly removed all -->
<!-- references to the the environment variable from core parts of Grail. DK-->

4. Edit `~/.bashrc` to include `export GRAIL=/path/to/GRAIL` and
   `export PATH=$PATH:$GRAIL/bin` on the following line. `source ~/.bashrc` the
   first time.
  
 If the MSYS2 method does not work, or there are issues, try the method below.
 1. Install Ubuntu 20.04 LTS from the Microsoft Store.
 2. Initially run the Ubuntu terminal once it is finished installing.
 3. In Windows Powershell type the following commands `wsl --install` `wsl --set-default Ubuntu-20.04` `wsl --set-version Ubuntu-20.04 2` Note: If you have an error about version mismatch, use command `wsl -l` to show what the name of the Ubuntu LTS is.
 4. Download the x server [here](https://sourceforge.net/projects/vcxsrv/). To finish the install click Next -> Next -> Finish.
 5. Once installation is complete, create a desktop shortcut for the server and in the Properties, target field type in `"C:\Program Files\VcXsrv\vcxsrv.exe" :0 -ac -terminate -lesspointer -multiwindow -clipboard -dpi auto` and apply the changes.
 6. 

## Getting Set Up - Ubuntu

### Ubuntu 18.04

1. Install gcc-11 and g++-11

   ```shell
   sudo add-apt-repository ppa:ubuntu-toolchain-r/test
   sudo apt-get update
   sudo apt-get install gcc-11 g++-11
   ```

   - Preferred, but optional

     1. Set gcc-11 and g++-11 as default compiler

        ```shell
        sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 11
        sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 11
        ```

     2. Confirm that the default versions of gcc and g++ are correct

        ```shell
        sudo update-alternatives --config gcc
        sudo update-alternatives --config g++
        ```

2. If the default version of CMake is installed, purge it from `apt`

   ```shell
   sudo apt purge --auto-remove cmake
   ```

3. Install the latest version of CMake

   ```shell
   wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
   echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
   sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
   sudo apt update
   sudo apt install kitware-archive-keyring
   sudo apt update
   sudo apt install cmake
   ```

4. Install other dependencies

   ```shell
   sudo apt install ninja-build libglfw3-dev libfreetype6-dev mpv libmpv-dev liblzma-dev flex bison pkg-config
   ```

5. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to
   set up the environment variables.

### Ubuntu 20.04

1. Install gcc-10 and g++-10

   ```shell
   sudo apt-get install gcc-10 g++-10
   ```

   - Preferred, but optional

     1. Set gcc-10 and g++-10 as default compiler

        ```shell
        sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10
        sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10
        ```

     2. Confirm that the default versions of gcc and g++ are correct

        ```shell
        sudo update-alternatives --config gcc
        sudo update-alternatives --config g++
        ```

2. Install dependencies

   ```shell
   sudo apt install make cmake ninja-build libglfw3-dev libfreetype-dev mpv libmpv-dev liblzma-dev flex bison pkg-config
   ```

3. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to
   set up the environment variables.

## Getting Set Up - Arch-Based Linux

1. Install dependencies

   ```shell
   sudo pacman -S glfw-x11 freetype2 base-devel cmake ninja mpv youtube-dl

   ```

   - If you are using Wayland, then install `glfw-wayland` instead of
     `glfw-x11`. Wayland support is currently unconfirmed.

2. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to
   set up the environment variables.

## Compiling

- To compile with default settings, run `./build.sh`
- For those who want to modify the default configuration and compile themselves,
  we use the following:

  ```shell
  cmake -S . -Bbuild -GNinja
  cmake --build build
  ```

- For versions of Ubuntu with custom versions of gcc, but did not set the
  default compiler with update-alternatives:

  ```shell
  CC=gcc-11 CXX=g++-11 cmake -S . -Bbuild -GNinja
  cmake --build build
  ```

## Running

1. Update `test/CMakeLists.txt` to compile the file you want to compile and link
   it to grail.
2. `cd test && testWidgets`

   - Cursors and other features might not work for other tests as they are not
     100% Linux/Windows compatible yet.

## Frequent Issues

### Need to create an SSH key to push into the repository

GitHub has a great guide on how to do this!
[This guide](https://docs.github.com/en/github/authenticating-to-github/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)
will show you how to load `ssh-agent` when your terminal starts and add a new
SSH key to `ssh-agent` and GitHub

### Cloned the repository using HTTPS, but now have to push to the repository

In this instance, you'll need to change the remote url that git is pointing to.
This can be done with a single git command that changes which website the remote
called `origin` is pointing at. Once you run this, try to push again and you
should be good to go!

Git command:

```shell
git remote set-url origin git@github.com:StevensDeptECE/GrailGUI.git
```

### Runtime Error: `Failed to open GLFW window`

This error is most commonly seen when attempting to run Grail on a Windows
virtual machine. If your use-case is different, please open an issue with the
details of your environment. As far as the project team is aware, this error can
be traced to a lack of OpenGL compatibility on Windows virtual machines. This
means that the issues is a driver problem that cannot be resolved until there is
better OpenGL support in virtual video drivers. If you need a virtual machine,
but do not need Windows, a virtual machine running Ubuntu Linux should work.
Otherwise, until there is sufficient OpenGL support for virtual Windows, you
will have to resort to a dual boot or alternative computer to run Grail and
Grail-based programs.

### git: 'remote-https' is not found or can't run program

This error could be a antivirus software quarantining your files. Avast is a 
common software that will prevent your PC from  running the program properly.
To fix this, go into the antivirus software and add an exception to the 
git-remote-https.exe file so that your computer can access it. 

### Everything Else

If you have an issue not on this list, and feel it should be included in the
list of frequent issues, open an issue and let us know!
