# Peter's Windows Port

## Getting Set Up - Windows

1. [Install MSYS2](https://www.msys2.org/)

2. Run MSYS2: `MSYS2 MinGW 64-bit` from the Windows Start Menu.

    - Make sure you are running the correct executable or else you will have trouble with the C++ compiler not recognizing your system correctly.

3. Install dependencies

    ``` shell
    pacman -S git nano make mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw64-w64-x86_64-cmake mingw64-w64-x86_64-ninja mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw mingw-w64-x86_64-mpv mingw-w64-x86_64-youtube-dl bison flex
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

2. If the default version of CMake is installed, purge it from `apt`

    ``` shell
    sudo apt purge --auto-remove cmake
    ```

3. Install the latest version of CMake

    ``` shell
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null
    sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
    sudo apt install kitware-archive-keyring
    sudo apt update
    sudo apt install cmake
    ```

4. Install other dependencies

    ``` shell
    sudo apt install ninja-build libglfw3-dev libfreetype6-dev mpv libmpv-dev liblzma-dev flex bison pkg-config
    ```

### Ubuntu 20.04

1. Install dependencies

    ``` shell
    sudo apt install make cmake ninja-build libglfw3-dev libfreetype-dev mpv libmpv-dev liblzma-dev flex bison
    ```

2. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

## Getting Set Up - Arch-Based Linux

1. Install dependencies

    ``` shell
    sudo pacman -S glfw-x11 freetype2 base-devel cmake ninja mpv youtube-dl

    ```

    - If you are using Wayland, there install `glfw-wayland` instead of `glfw-x11`. Wayland support is currently unconfirmed.
  
2. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

## Compiling

- To compile with default settings, run `./build.sh`
- For versions of Ubuntu with custom versions of gcc (such as Ubuntu 18.04), you must specify the compiler when the project is generated:

    ``` shell
    CC=gcc-11 CXX=g++-11 cmake -S . -Bbuild -GNinja
    cmake --build build
    ```

- For those who want to modify the default configuration and compile themselves, we use the following:

    ``` shell
    cmake -S . -Bbuild -GNinja
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

        git remote set-url origin git@github.com:StevensDeptECE/GrailGUI.git

### Compile Time Error:  `Makefile:1: /proj/settings.mk: No such file or directory`

The `GRAIL` environment variable is most likely not set. Please refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows). If you have done this and still find yourself getting the same error, you may using a shell besides Bash. This is important because the instructions provided tell you to edit the `.bashrc`, but that file may not be loaded if you were zsh, ksh, fish, or another alternative. To check which shell you are running, use one of the solutions mentioned [here](https://stackoverflow.com/a/3327022) and then look up which file you would edit to export environment variables.

### Runtime Error:  `Failed to open GLFW window`

This error is most commonly scene when attempting to run Grail on a Windows virtual machine. If your use-case is different, please open an issue with the details of your environment. As far as the project team is aware, this error can be traced to a lack of OpenGL compatibility on Windows virtual machines. This means that the issues is a driver problem that cannot be resolved until there is better OpenGL support in virtual video drivers. If you need a virtual machine, but do not need Windows, a virtual machine running Ubuntu Linux should work. Otherwise, until there is sufficient OpenGL support for virtual Windows, you will have to resort to a dual boot or alternative computer to run Grail and Grail-based programs.

### Everything Else

If you have an issue not on this list, and feel it should be included in the list of frequent issues, open an issue and let us know!
