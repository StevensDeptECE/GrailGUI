<!-- omit in toc -->

# Grail

GrailGUI is a prototype of a replacement for web programming (HTTP, HTML, CSS,
JavaScript) and GUI programming. It includes a client graphical API, a browser
implemented in C++ and OpenGL, a protocol to transmit metadata and data in
binary, a language (XDL) to describe the binary data, and local storage to
retain data on the client should that be necessary. Encrypted communications
(equivalent of TLS) have not yet been implemented.

For more info see [our writeup from Summer 2021.](docs/Grail.md)

## Summer 2023 TODO's

We will be tracking things we want to work on using GitHub projects. Check the
projects tab to see open tickets on what needs to be worked on.

## How to Build Grail For Development

The easiest way to build Grail is to use the Dockerfile we provide to build
yourself a Debian 11 container with all of the necessary dependencies. This
process has been tested to work on Linux, but futher testing is required for
Windows and possibly MacOS.

If you don't have Docker installed, head to
[https://docs.docker.com/install/](https://docs.docker.com/install/) and follow
the instructions there.

### Building the Image

In the root directory of this repository, run:

```bash
docker build --build-arg=UID=$(id -u) --build-arg=GID=$(id -g) -t grail .
```

This will take some time, so just let it do its thing.

### Running and Entering the Container

In the root directory of this repository, run:

```bash
docker compose up -d # run the container
xhost +local: # allow the container access to your X11 display
docker compose exec build_env bash # enter the container
```

The first command brings the container online, the second command ensures that
the container can access your display, and the third runs bash inside the
container. This container should run until you shut down your machine. If you'd
like to turn it off manually, run the following in the root of the repository:

```bash
docker compose down
```

### Building and Running Grail From Within the Container

Once you've entered the container, you should be greeted with a standard bash
shell. You should already be in a folder containing the source code for Grail,
and you can build it with our standard settings with the following:

```bash
./build.sh
```

Alternatively, if you know cmake and want to use custom options for the build,
you can run:

```bash
cmake -S . -Bbuild -GNinja
cmake --build build
```

To run a test (using globe as an example):

```bash
cd test
../bin/Globe
```

## How to Build Grail For Deployment

<!-- TODO Do this -->

This is still a work in progress, but we are working on support for installing
Grail to your system, and then building programs which link to it easily.

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
common software that will prevent your PC from running the program properly. To
fix this, go into the antivirus software and add an exception to the
git-remote-https.exe file so that your computer can access it.

### Everything Else

If you have an issue not on this list, and feel it should be included in the
list of frequent issues, open an issue and let us know!
