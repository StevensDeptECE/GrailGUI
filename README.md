# Grail

GrailGUI is a prototype of a replacement for web programming (HTTP, HTML, CSS,
JavaScript) and GUI programming. It includes a client graphical API, a browser
implemented in C++ and OpenGL, a protocol to transmit metadata and data in
binary, a language (XDL) to describe the binary data, and local storage to
retain data on the client should that be necessary. Encrypted communications
(equivalent of TLS) have not yet been implemented.

For more info see [our writeup from Summer 2021.](docs/Grail.md)

<!-- omit in toc -->

## Table of Contents

<!--toc:start-->

- [Grail](#grail)
  - [Table of Contents](#table-of-contents)
  - [Summer 2023 TODO's](#summer-2023-todos)
  - [How to build Grail on Linux and Windows](#how-to-build-grail-on-linux-and-windows)
    - [Windows](#windows)
      - [MSYS2](#msys2)
      - [Windows Subsystem for Linux (WSL2)](#windows-subsystem-for-linux-wsl2)
    - [Linux](#linux)
      - [Debian and Ubuntu](#debian-and-ubuntu)
      - [Arch Linux](#arch-linux)
    - [Compiling](#compiling)
    - [Running](#running)
  - [Frequent Issues](#frequent-issues)
    - [Need to create an SSH key to push into the repository](#need-to-create-an-ssh-key-to-push-into-the-repository)
    - [Cloned the repository using HTTPS, but now have to push to the repository](#cloned-the-repository-using-https-but-now-have-to-push-to-the-repository)
    - [Runtime Error: `Failed to open GLFW window`](#runtime-error-failed-to-open-glfw-window)
    - [git: 'remote-https' is not found or can't run program](#git-remote-https-is-not-found-or-cant-run-program)
    - [Everything Else](#everything-else)

<!--toc:end-->

## Summer 2023 TODO's

We'll be managing the things we want to get done with GitHub Issues and Pull
Requests. Check there for a good place to start contributing.

## How to build Grail on Linux and Windows

We are using C++20 features, so you'll need a compiler with full C++17 support
and a decent amount of C++20. Our current recommendation is gcc-10, but up to
date versions of clang should be fine as well.

We use CMake to build the project, you'll need vertion 3.16 or later.

We vendor some of our dependencies either directly committed to version control,
or pulled from GitHub with CMake fetchcontent. The dependencies you'll need from
your system package manager are the following:

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

### Windows

#### MSYS2

1. [Install MSYS2](https://www.msys2.org/)

You shouldn't need to do anything special during the install, just keep clicking
next.

2. Run MSYS2: `MSYS2 UCRT64` from the Windows Start Menu.

This shell should have a yellow / gold MSYS2 logo. Make sure you're using this
one in the future, as if you run the wrong one your C++ compiler will have
trouble finding the right libraries.

3. Install dependencies

```bash
pacman -Syyu
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S pactoys
pacman -S git
pacboy -S cmake:p
pacboy -S freetype:p
pacboy -S glfw:p
pacboy -S mpv:p
pacboy -S youtube-dl:p
pacboy -S github-cli:p
pacboy -S gdb:p
```

4. Authenticate with GitHub CLI

If you know how to add an ssh key to GitHub, and you've done it for MSYS2 before
then you can skip this step. If you have no idea what ssh is or how to add a key
to GitHub, just use the cli tools for authentication.

```bash
gh auth login
```

Just follow the prompts and you'll be good to go.

5. Optional: Setup the GRAIL Environment Variable

The way sources in Grail are layed out means that you'll need to be in the
`test/` directory to run the tests, but when the project builds the test
executables will be places in `bin/`. You can deal with this in a couple of
ways:

- Just use relative paths

```bash
# when you're in the test folder
../bin/<executable you want to run here>
# example
../bin/Globe.exe
```

- Add an environment variable

Edit `~/.bashrc` to include `export GRAIL=/path/to/GRAIL` and
`export PATH=$PATH:$GRAIL/bin` on the following line. Restart your terminal that
should be it.

If the MSYS2 method does not work, or there are issues, you can try the method
below, but support for it will be limited.

#### Windows Subsystem for Linux (WSL2)

1. Follow [this guide](https://docs.microsoft.com/en-us/windows/wsl/install) to
   install WSL.
2. In Windows Powershell, type the following command to install Ubuntu 20.04 as
   the default WSL distribution.

   ```bash
   wsl --install -d Ubuntu-20.04
   wsl --set-version Ubuntu-20.04 2
   ```

   - Note: If you have an error about version mismatch, use command `wsl -l` to
     show what the name of the Ubuntu LTS is.

3. Download the x server [here](https://sourceforge.net/projects/vcxsrv/). To
   finish the install click Next -> Next -> Finish.
4. Once installation is complete, right click on the desktop and create a new
   shortcut.
5. In the shortcut's properties, fill in the target field with the following and
   apply the changes

   ```powershell
   "C:\Program Files\VcXsrv\vcxsrv.exe" :0 -ac -terminate -lesspointer -multiwindow -clipboard -dpi auto
   ```

6. Double-click the shortcut to run VcXsrv, there should now be an icon for it
   in your taskbar's tray.
7. In the WSL2 shell, edit the `~/.bashrc` file and add the following line. This
   allows WSL2 to connect to VcXsrv.

   ```bash
   export DISPLAY="`grep nameserver /etc/resolv.conf | sed 's/nameserver //'`:0"
   ```

8. To test VcXsrv, run the following commands. These should make a pair of eyes
   appear on your screen, followed by a calculator.

   ```bash
   sudo apt install x11-apps
   xeyes
   xcalc
   ```

9. Install dependencies by referring to the
   [Debian and Ubuntu setup](#debian-and-ubuntu).
10. Edit the `~/.bashrc` file the same way as the [step above](#msys2).

### Linux

#### Debian and Ubuntu

The oldest Linux distribution we've tested to build successfully on is Debian 11
Bullseye. It has a C++20 compatible compiler and recent enough versions of
packages. Anything derived from it should be fine, as well as somewhat recent
versions of Ubuntu. Run the following to get everything you should need:

```bash
sudo apt update && sudo apt install \
    build-essential \
    cmake \
    ca-certificates \
    git \
    ninja-build \
    libglfw3-dev \
    libfreetype-dev \
    libmpv-dev \
    liblzma-dev \
    pkg-config
```

As with MSYS2, optionally edit your `~/.bashrc` as described [here](#msys2).

#### Arch Linux

1. Install dependencies

```shell
sudo pacman -S glfw-x11 freetype2 base-devel cmake ninja mpv youtube-dl
```

If you are using Wayland, then install `glfw-wayland` instead of `glfw-x11`.
Wayland support is currently unconfirmed.

2. Refer to step 4 of [MSYS2](#msys2) to set up the environment variables.

### Compiling

To compile one test with default settings, run `./build.sh <testname>`

To compile all tests with default settings, run `./build.sh`

For those who want to modify the default configuration and compile themselves,
we use the following:

```shell
cmake -S . -Bbuild -GNinja
cmake --build build
```

For building performance-tuned with optimization with debug:

```shell
cmake -Bbuild -GNinja -DCMAKE_BUILD_TYPE=relwithdebinfo
cmake --build build
```

For building performance-tuned with optimization without debugging information:

```shell
cmake -Bbuild -GNinja -DCMAKE_BUILD_TYPE=release
cmake --build build
```

### Running

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
SSH key to `ssh-agent` and GitHub. If you've authenticated with GitHub CLI, then
no need to worry about doing this.

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
details of your environment.

As far as the project team is aware, this error can be traced to a lack of
OpenGL compatibility on Windows virtual machines. The virtual video drivers are
only compatible with the ancient OpenGL 1.1, and we're using much more modern
version of OpenGL. We've tried installing newer standalone video drivers and
Grail programs are able to launch, but immediately segfault once shaders are
loaded.

This means that the issue is a driver problem that cannot be resolved until
there is better OpenGL support in virtual video drivers. If you need a virtual
machine, but do not need Windows, a virtual machine running Ubuntu or Debian
should work. Otherwise, until there is sufficient OpenGL support for virtual
Windows, you will have to resort to a dual boot or alternative computer to run
Grail and Grail-based programs.

### git: 'remote-https' is not found or can't run program

This error could be a antivirus software quarantining your files. Avast is a
common software that will prevent your PC from running the program properly. To
fix this, go into the antivirus software and add an exception to the
git-remote-https.exe file so that your computer can access it.

### Everything Else

If you have an issue not on this list, and feel it should be included in the
list of frequent issues, open an issue and let us know!
