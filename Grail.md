# Grail Project Report

## Introduction and Background

Hello and welcome to Grail! My name is Dov Kruger, I am a teaching associate professor at Stevens. This section describes how I came up with the
concept behind Grail, and how a collection of fantastic students has taken the project to where it is today.

Grail started as a concept to see how efficient and simple a web server could be, and then build security into the engine rather than expect
application programmers write their own security. This was based on my experience developing in Java Servlets and JSP. I called it C++ Server pages (CSP).

The original project stripped down lighthttpd to the bone, implementing in very tight C++, and could execute in-memory C++ servlets.
It became clear that the most performance could be improved was a small multiple on the server, because the limiting factor is network performance.
The drop in CPU utilization was quite impressive. We could run easily in 60k, and since we required so little CPU we could achieve high speed on small CPUs, but the technology would not be transformative.

The next conceptual leap was to define a new way of writing web programs, with the specific goal of making transactional web sites
(the kind you need to create a login for) to make the data transmission more efficient. There are two main keys to 
web performance: send data in binary, so that the server does less work, latency is reduced and bandwidth is reduced. Second, and more important,
send less data! The key to sending less data is to store data locally on the client, and not ask again for data the client already has. This is not
caching. Instead, a web app should be allocated storage, and it should store data locally. This local storage must of course be kept secure, so encryption is vital. Right now, the only way to implement this approach would be writing a custom app which is both expensive and not portable. There are some portable toolkits.

To defend against phishing attacks, CSP was conceived to automatically create public/private keypairs for each website. The user never logs into a website. Instead the user types in their password to unlock the internal "real" password for the site, which they do not know. This concept is still not implemented and there is a great deal more to it, beyond the scope of this introduction.

This is where the concept stood in early 2019. At this point, thinking about how web programming had largely displaced most GUI development, 
it occurred to me that if a web programming system could be made efficient enough, perhaps there could be only a single, portable GUI model.
It could work across multiple languages. For any menu-based program, the GUI doesn't even need to be in the same memory space as the executable.
A gui merely has to bind functions in a target executable to input events.
A thought came, unbidden "A single GUI which would work on the web or linked into an application like a CAD package would be the holy grail for user-interface design." That's the origin of the name Grail.

Today, we are focusing on the client of Grail, implementing in C++ and OpenGL to create an engine that renders from 10 to 100 times faster than
the Chrome browser, that uses a tiny fraction of the memory of a browser, that transmits data in binary using a data definition language called XDL, derived from IDL used in CORBA. A tiny codebase (630kb) delivers the ability to render text, 2d graphics, 3d graphics, animation and more.
We have widgets defined which can graph data as an example, and we are working on GUI widgets like menus.

## Goals of Grail

1. Develop a single coherent, high-efficiency kernel supporting graphical applications, and a transport layer supporting secure network applications.
1. Create a secure web browser and server that does not allow execution of arbitrary local code, no JavaScript but still provides at least
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
   Implemented the font system, mentor new programmers and review code. Implemented input event mapping system.
1. Peter Ho - Student 2019 summer. Built the core web engine, implemented the first input event mapping system, and helped debug OpenGL code.
1. Itai Bachar - High School intern 2019 Summer who pioneered some of the OpenGL code.
1. Alex Walker - Senior Design 2019 implemented 3d models and shaders.
1. Pridhvi Myneni - Student 2020 summer. Cleaned up GLWin, ported to Windows, implemented XDL transport 
1. Joey Rupert - Ported to Windows, Managed team
1. Alice Huston - Student 2021. Debugged many areas, supported other programmers, rewrote the project build using cmake and ninja, ported to C++ 20,
   wrote BlockLoaders for high speed I/O, Implemented the map and shapefile loader. Implemented LZMA decryption.
1. Luisa Bonfim - Student 2021. Implemented multiple graph widgets including SparkLine, BarChart, Candlestick, GapMinderGraph, 
1. David Krauthamer - Student 2021. Implemented LineGraph, axes, Video and audio player. Implementing registration for input events directly to objects.
1. Kamen Kresnitzki - Student 2021. Co-authored some of the above with David.
writing StyledMultiShape2.5D to support layered 2D shapes, and creating and testing a BlockLoader (GapMinderLoader) to rapidly load
data from the GapMinder project to graph.
1. Nikola Ciric - Student 2021. Implementing solarsystem animated display.
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
