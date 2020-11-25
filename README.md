# Peter's Windows Port

## Getting Set Up - Windows

 1. [Install MSYS2](https://www.msys2.org/)
 2. Run MSYS2: `MSYS2 MinGW 64-bit` from the Windows Start Menu.

    - Make sure you are running the correct executable or else you will have trouble with the C++ compiler not recognizing your system correctly.

 3. `pacman -S git nano make cmake mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw bison flex`

     - If you are having troubles: the full list I installed was: `pacman -S git nano make cmake mingw64/mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb mingw-w64-x86_64-gcc mingw-w64-x86_64-toolchain mingw-w64-x86_64-zlib mingw-w64-x86_64-freetype mingw-w64-x86_64-glfw bison flex python pkg-config mingw-w64-x86_64-gcc mingw-w64-x86_64-eigen3 mingw-w64-x86_64-qt5`

 4. Edit `~/.bashrc` to include `export GRAIL=/path/to/GRAIL` and `export PATH=$PATH:$GRAIL/bin` on the following line. `source ~/.bashrc` the first time.

      - Example: `/c/Users/USERNAME/Documents/GitHub/Grail`
 5. Install PLibSys

    1. Download from [PLibSys Releases](https://github.com/saprykin/plibsys/releases/). Get Latest `Source Code (zip)`
    2. Decompress Locally
    3. Run `cmake CMakeLists.txt`
    4. `make`
    5. `make install` (need sudo in linux)

## Getting Set Up - Linux

1. Install Dependencies: `sudo apt install libglfw3-dev libfreetype-dev flex bison`

## Compiling

1. `cd $GRAIL/ ; make`

## Running

1. Update `$GRAIL/test/Makefile` to compile the file you want to compile.
2. `cd $GRAIL/test/ && testDrawing`

     - Cursors and other features might not work for other tests as they are not 100% Linux/Windows compatible yet.
