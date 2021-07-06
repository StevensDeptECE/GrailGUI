# Block Loaders

## Introduction

A blockloader is a file format intended to efficiently load a data structure into memory using minimal computation and time.
The simplest form of a blockloader is a file designed to load completely into memory in a single read.
A second form may load in multiple chunks, with space allocated between the chunks providing room to grow. This is called a GrowableBlockLoader.
Data types using pointers cannot be written directly to disk because the pointers have meaning only for a particular run instance.
For such data types, the way to make them automatically serializable is to replace pointers by integer byte offsets.
This has two advantages: first, 32-bit integers are half the size of pointers on a 64-bit computer, and second, the data structure can be
block loaded into memory with no subsequent fixup required.

There are three advantages of the BlockLoader design and proposed standard:

  1. Provide an efficient mechanism for loading (and potentially saving) data.
  2. Provide a uniform file format where header information is processed in a standard way, so the learning curve and programming 
     is reduced for all block loaders because they all have a uniform header format.
  3. Provide standardized code for authentication, copyright, and other issues to reduce code complexity.

## BlockLoader Format

Every blockloader contains a standard header. The first part, the GeneralHeader defines which other parts follow, so that optional components
may not be present.

  1. The structure of a blockloader contains
    - GeneralHeader
    - SecurityHeader
    - HashMapHeader: TODO: maybe more general than this
    - 
  2. GeneralHeader
    - The general header is defined in C++ as follows:
  3. SecurityHeader
    - The security header contains a digital signature signing the file, and the id
    - of the person claiming authorship and responsibility for the document.
  4. A list of BlockLoaderComponents. A BlockLoader file must contain at least one

    - Make sure you are running the correct executable or else you will have trouble with the C++ compiler not recognizing your system correctly.

 1. `pacman -S git nano make cmake mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw mingw-w64-x86_64-mpv mingw-w64-x86_64-youtube-dl bison flex`

     - If you are having troubles: the full list I installed was: `pacman -S git nano make cmake mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw bison flex python pkg-config mingw-w64-x86_64-gcc mingw-w64-x86_64-eigen3 mingw-w64-x86_64-qt5 mingw-w64-x86_64-mpv mingw-w64-x86_64-youtube-dl`

 2. Edit `~/.bashrc` to include `export GRAIL=/path/to/GRAIL` and `export PATH=$PATH:$GRAIL/bin` on the following line. `source ~/.bashrc` the first time.

## Getting Set Up - Ubuntu

1. Install Dependencies: `sudo apt install make libglfw3-dev libfreetype-dev mpv libmpv-dev flex bison`

2. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

## Getting Set Up - Arch-Based Linux

1. Install Dependencies: `sudo pacman -S glfw-x11 freetype2 base-devel mpv youtube-dl`

    - If you are using Wayland, there install `glfw-wayland` instead of `glfw-x11`. Wayland support is currently unconfirmed.
  
2. Refer to step 4 of [Getting Set up - Windows](#getting-set-up---windows) to set up the environment variables.

## Compiling

1. `cd $GRAIL/ ; make`

## Running

1. Update `$GRAIL/test/Makefile` to compile the file you want to compile.
2. `cd $GRAIL/test/ && testDrawing`

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

If you have an issue not on this list, and feel it should be included on the list of frequent issues, open an issue and let us know!
