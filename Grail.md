# Grail Project Report

## Introduction and Background

Hello and welcome to Grail! My name is Dov Kruger, I am a teaching associate professor at Stevens. This document describes how I came up with the
concept behind Grail, and how a collection of fantastic students has taken the project to where it is today.

Grail started as a concept to see how efficient and simple a web server could be, and then build security into the engine rather than expect
application programmers write their own security. This was based on my experience developing in Java Servlets and JSP. I called it C++ Server pages (CSP).

The original project stripped down lighthttpd to the bone, implementing in very tight C++, and could execute in-memory C++ servlets.
It became clear that the most performance could be improved was a factor of 2 or 3, not even in all cases, because the limiting factor is network performance not server CPU.
The drop in CPU utilization was quite impressive, however. We could run easily in 60k, and since we required so little CPU we could achieve high speed on small CPUs like a Raspberry Pi, we could make servers that would run on a router with minimal CPU but the technology would not be transformative.

The next conceptual leap was to define a new way of writing web programs, with the specific goal of making transactional web sites which require a login
more efficient. There are two main keys to 
web performance: send data in binary, so that the server does less work, latency is reduced and bandwidth is reduced. Second, and more important,
send less data! The key to sending less data is to store data locally on the client, and not ask again for data the client already has. This is not
caching. Instead, a web app should be allocated storage, and it should store data locally. This local storage must of course be kept secure, so encryption is vital. Right now, the only way to implement this approach is to write a custom app which is both expensive and not portable.

To defend against phishing attacks, CSP was conceived to automatically create public/private keypairs for each website. The user never logs into a website. Instead the user types in their password to unlock the internal "real" password for the site, which they do not know. This concept is still not implemented and there is a great deal more to it, beyond the scope of this introduction.

This is where the concept stood in early 2019. At this point, thinking about how web programming had largely displaced most GUI development, 
it occurred to me that if a web programming system could be made efficient enough, perhaps there could be only a single, portable GUI model.
It could work across multiple languages. For any menu-based program, the GUI doesn't even need to be in the same memory space as the executable.
A gui merely has to bind functions in a target executable to input events. In Java, the libraries at the lowest level can call C++ through Java Native Interface (JNI) calls but it is inefficient, and the memory structure of Java is less efficient than C++. Instead, why not call out on a more granular level and keep more of the graphics in C++, not just individual OpenGL calls?

A thought flashed through my head: "A single GUI which would work on the web or linked into an application like a CAD package would be the holy grail for user-interface design." That's the origin of the name Grail.

Today, we are focusing on the client of Grail, implementing in C++ and OpenGL to create an engine that renders from 10 to 100 times faster than
the Chrome browser, that uses a tiny fraction of the memory of a browser, that transmits data in binary using a data definition language called XDL, derived from IDL used in CORBA. A tiny codebase (now up to 840kb, with another 100kb or so not yet integrated into this repository) delivers the ability to render text, 2d graphics, 3d graphics, animation and more.
We have widgets defined which can graph data as an example, and we are working on GUI widgets like menus.

At the moment, GrailGUI is focused on the drawing primitives and data transmission in XDL. There is no web engine in this implementation, though a high performance compiled web engine was designed and built in an earlier Grail project which is not released as open source.

## Efficiency and Simplicity

The central concept of Grail is to demonstrate the maximum efficiency possible in a GUI API, and yet simultaneously to make programming much simpler. In order to accelerate graphics, OpenGL and its descendant Vulkan have developed a programming model where a program on the graphics card (a shader) communicates with a program on the computer (often written in C or C++). Turning what used to be a single program into two programs in different languages communicating drastically increases code complexity and raises a gigantic barrier to entry. GrailGUI implements classes (StyledMultiShape2D, MultiText, StyledMultiShape3D) designed to allow programmers to render graphics optimally for a wide range of cases without writing shaders. OpenGL is still exposed, and a programmer can write shaders, but the hope is to avoid shaders most of the time.

In addition, as the Grail API has developed, I have constantly looked for opportunities to simplify the programming model and reduce the size of the code. The key
areas that are repetitive in most GUI APIs can be summarized as:

1. Building a window structure with a hierarchical set of objects
1. Binding input events (keyboard and mouse) to actions
1. Animation

The first, most important job is to build an API that supports all the operations desired. As Alan Kay suggested in developing the concept of the Dynabook, having a single coherent API can dramatically simplify programming. It is amazing how much smaller and more efficient code can be made when there is a single API. Once defining the API, however, we can also make sure that there are facades designed to make sure that typical use cases require minimal code.
GrailGUI has already demonstrated this in many ways: Our typical executable is quite small (5Mb), and our demos illustrate that we can generate an impressive application in just a few lines. For example, the animated globe demo currently takes just 31 lines of code. The processing library is the only API of which we are aware that is simpler than GrailGUI, but it is designed for learning and ease of use, and is wildly inefficient. We have achieved similar simplicity without losing the inherent efficiency of the underlying OpenGL and without forcing the user to encounter shaders unless they need some feature that the general purpose API does not provide.

In the future, with a user community expanding the library, it seems likely that very few graphical programs would have to encounter shaders at all.

I have observed that there are families or categories of GUI programs including 2D drawings, 3D rendering, documents. For each of these categories, a set of input operations is desired. For example a 2d map viewer has operations like zoom in and out and panning. A Document has operations like goto top, bottom, page up, page down. By predefining sets of these operations, the amount of work to implement a document or a map viewer is dramatically reduced.

Richard Stallman wrote that by implementing GNU, companies that implemented operating systems would be unhappy, but that everyone else would benefit. Grail has a similar purpose. By creating a single clean API providing a toolkit for writing 2D, 3D, document and other classes of GUI apps, it should be possible to create a single program that will port to multiple platforms. This will not make operating system vendors happy. At the moment, Windows still dominates largely based on network effects, with applications such as CAD packages typically working only on Windows. While Grail won't force software vendors to port, by reducing the barrier to porting dramatically we can make it far less expensive. This in turn would mean that there would be less reason not to provide a program on multiple operating systems. 


## Implementation Language and APIs: C++, Rust, OpenGL, Vulkan, GLFW, SDL

In the long run, C++ is too complex and error prone and something better, more reliable and simpler must replace it. Java has been closing the gap in terms of performance, but has a clunky template system, has a virtual machine that lags current computer architectures because it locked in 25 years ago (no 128 bit types for example). Rust seems very good but I simply don't know it well enough. At the moment, research and implementation in Grail are tied together, and therefore use C++. In the future I envision porting to a new language, perhaps Rust unless something better comes along. However, the most important feature is not the language Grail is written in, but the design of the API. Once the structure of Grail is fully defined, porting will be comparatively easy. As we have developed the system, the API has been in flux as we keep finding ways to reduce complexity and shrink the size of typical programs.

THe same applies to Vulkan. However poorly we know OpenGL, Vulkan is an even bigger obstacle. The goal is to create an API at the top level and allow programming at the shader level, and that should eventually move to Vulkan. The big question right now is what to do to port to Mac OSX, where they have deprecated OpenGL. Eventually, it would conceivably be the job of the Grail maintenance team to maintain a portable layer that would work on each operating system. That is the price that must be paid to liberate all programmers from the tyrrany of different APIs such as metal on Mac OSX.

## Goals of Grail

The goal of grail is to make GUI and web development as simple and portable as writing a console program in a typical language. Java and C# provide a partial solution, but at a huge cost: Java for example uses OpenGL in its GUI toolkit internally, but will not interoperate with it. A toolkit such as LWJGL (Lightweight Java Gaming Library) provides access to OpenGL but then all the GUI objects will not work in the same window. There are also some speed penalties for using Java, and while ordinary code performance is closing the gap, Java object layout and memory management are intrinsically less efficient.

1. Develop a single toolkit that will build executables on Linux, Windows, MacOSX, Android and iOS.
1. Develop a single coherent, high-efficiency kernel supporting graphical applications, and a transport layer supporting secure network applications.
1. Create a secure web browser and server that does not allow execution of arbitrary local code, like JavaScript but still provides at least
at equivalent experience for a user of a transactional website, while providing very high performance 2D and 3D rendering.
1. Provide access to full OpenGL control of video performance, but create optimal drawing wrapper objects for common cases like 2d drawings on a web page that do not require knowledge of the OpenGL layer.
1. Create a document object that supports uniform, multiple kinds of media at scale, rendering instantly.
1. Create an editor capable of editing large-scale (1 Gb+) documents with instant response.
1. Add video and audio capability so that Grail programs can display movies and implement video chat systems.
1. Create a security engine that automatically generates public/private keys and manages logins to websites so that passwords are not used to log in to web sites. 
1. Maintain a repository of passwords encrypted on the client that can be securely synchronized with other clients and backed up to the cloud.
1. Create a uniform method of multiple user inputs via the netowork so that any grail can invite other grail users to use keyboard and mouse and type collaboratively.
1. Create a set of user interface widgets to implement buttons, scrollbars, menus, and web links that are portable in OpenGL.
1. Create an efficient rendering engine for multibyte characters (Chinese, Japanese, Korean).
1. Create a text rendering engine supporting equations.
1. Reduce the CPU load of a server by a factor of 1000, meaning that the number of servers can be reduced by a factor of 100 while still dramatically improving performance. This could actually have a measurable effect on world electricity consumption if it were adopted by major
transactional web systems.

Currently, the world uses web browsers and JavaScript as the engine to achieve all the above. Unfortunately web browsers as currently implemented are a security nightmare. Arbitrary JavaScript code, run on browsers is a prescription for theft of data, privacy invasion, and malware. JavaScript is a huge security problem, and a factor of 1000 slower than Grail in many cases. The codebase for chrome is 30Gb. It is safe to say that no one understands what is in it, and since people can add plugins that can intercept requests, it is not safe to use Chrome for banking and other high-security applications. Since performance of CPUs is no longer scaling rapidly, re-implementing the core engine of the web can not only make the internet a more secure place, it can provide the boost in productivity and performance that hardware is no longer providing. 

## Development History

## Grail Volunteers
The following list is not comprehensive. It contains the major contributors to Grail, and all the current students working this summer.

1. Dov Kruger - Project Lead, Teaching Assistant Professor at Stevens Institute of Technology, ECE Department.
   Invented grail, designed bytecode web engine, OpenGL wrappers, defined the concept of BlockLoaders and wrote the first one,
   Implemented the font system, efficient wrappers for OpenGL, mentor new programmers and reviewed most code in the system.
   Implemented unified input event mapping system.
1. Peter Ho - Student 2019 summer. Built the core web engine, implemented the first input event mapping system, and helped debug OpenGL code.
1. Itai Bachar - High School intern 2019 Summer who pioneered some of the OpenGL code.
1. Alex Walker - Senior Design 2019 implemented 3d models and shaders.
1. Pridhvi Myneni - Student 2020 summer. Cleaned up GLWin, ported to Windows, improved the makefile. Implemented XDL transport.
1. Joey Rupert - Ported to Windows, Managed team
1. Alice Huston - Student 2021. Debugged many areas, supported other programmers, rewrote the project build using cmake and ninja, ported to C++ 20,
   wrote BlockLoaders for high speed I/O, Implemented the map and ESRI shapefile loader. Implemented LZMA decryption. Use std::bind to make the
   bindings call direct to arbitrary objects not just the main window. Cleaned up Multishape hierarchy.
   Implemented continuous integration in github automatically testing Grail on Linux and windows in the cloud.
1. Luisa Bonfim - Student 2021. Implemented multiple graph widgets including SparkLine, BarChart, Candlestick, GapMinderGraph.
   Built GapMinder database blockloader supporting hundreds of variables to graph. Implementing StyledMultiShape2.5 dimensions.
1. David Krauthamer - Student 2021. Implemented LineGraph, axes, Video and audio player. Implementing registration for input events directly to objects.
1. Kamen Kresnitzki - Student 2021. Co-authored some of the above with David.
writing StyledMultiShape2.5D to support layered 2D shapes, and creating and testing a BlockLoader (GapMinderLoader) to rapidly load
data from the GapMinder project to graph.
1. Nikola Ciric - Student 2021. Implementing solarsystem animated simulation.
1. Quentin Jimenez - Student 2021. Button widget and demo.
1. Claudia Macrae - Student 2021. Implemented scrollbar.

## Project Documentation

A slideshow is available at 

The README describing how to build grail (currently on Linux and windows) is at



## Grail Demos and screenshots

Each of the following links will display source code and a screen shot of that demo.

1. SimpleDemo
1. testStyledMultiShape2D
1. Globe
1. SolarSystem
1. testDisplayBook
1. testVideo
1. testWebcam
1. testBlockMapViewer
1. testWidgets
1. testWidgetGapMinder
1. testSolarSystem2D
1. testDisplay3D
1. 

## 2021 Workshops: What students learned

In 2021, students took workshops in C++/OpenGL and in C++ and cryptography basics using OpenSSL. 
The following descriptions show what they learned and how it was applied in the Grail project this summer.

1. 
1. 
