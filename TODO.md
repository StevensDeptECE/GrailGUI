# Main Goals of GrailGUI Project

## Overview

 GrailGUI is set of C++ classes providing a simple wrapper for OpenGL with
 a standard set of objects and shaders that currently renders:
   1. 2D shapes
   1. 2D text,
   1. Images
   1. Support layers of objects (2.5D)
   1. Portable GUI widgets using OpenGL primitives
   1. 3D objects with textures


  GrailGUI also attempts to make many aspects of GUI development more compact
  and elegant. The current codebase is approximately 24k lines, 630kb.
  Object-Oriented demos are tiny, with minimal code needed to
  create a working application. While not as simple as processing, GrailGUI
  is designed to allow novice C++ programmers to achieve high-efficiency
  graphical applications more easily.

  Grail provides some standardized features designed to make developing
  simulations, games, GUI interfaces and data visualization tools.
   1. Predefined time and standardized commands to reset,speed-up, slow-down,
      jump forward in time.
   1. Ability to save screenshots
   1. Ability to save a movie (though very slow)
   1. Predefined move the camera
   1. Configuration for internationalization
   1. Network connection to servers using high-speed binary data using XDL,
   a new language designed with efficient binary data, metadata, and
   internationalization. We hope to write a compiler to generate code in
   C++ and other languages from XDL, making data accessible like Google
   protobuf, but retaining the ability to examine and display data generically
   without programming.

 The purpose of GrailGUI is to create graphical applications for visualizing
 data, for creating a portable GUI that will work on any platform using OpenGL,
 and by providing a compact, high-performance engine that can be accessed from
 any language.

 Eventual goals of Grail include
   1. Creating an efficient engine that can create a uniform graphical interface
   for any language (such as Java).
   1. Extensible components
   
 Grail is not a game engine, and it is not a goal that Grail should become one.
 However, as the project continues, if a game engine could be incorporated into
 Grail, or a subcomponent could be written, that would be great. The central
 theme of Grail is to dramatically lower the bar to entry for developing
 high-performance portable graphical applications, and to make the code for
 doing so clean and far smaller.

 As Grail grows, it is essential to maintain a single coherent, clean codebase
 that is manageable and understandable. Grail is currently simple, clean,
 and very, very fast (it does still have memory bugs).
 We could display a typical formatted page of text at
 8000fps on a GTX 2060, if there were a display capable of doing so, which
 means in practice that there is huge headroom to display vastly more
 complicated pages and still be faster than rendering engines for web browsers,
 pdf viewers, and book readers. Grail could power a new generation of such
 software.
 
## Portability

 GrailGUI is currently implemented on Linux (tested on Ubuntu and Centos)
 and Windows (at the time of writing, the sockets code has not yet been ported)
 We are currently porting to Raspberry Pi which should not be very difficult.
 More ambitious ports are below in TODO.

## TODO

 The following items are high on the TODO list. We are looking for capable
 C++, OpenGL, Android, iOS, and rust programmers to contribute.

 1. Porting to Mac OSX, Android, iphone. The critical thing is to make this
 absolutely invisible to the application programmer. There are big barriers
 to entry in writing iOS and Android apps. The goal of a port to those
 platforms is to provide a wrapper that will allow a developer to create a
 demo quickly and easily in Grail, without knowing anything at all about those
 platforms. Wrapper code can be in Kotlin on Android, and Swift on iOS, but
 should not be visible to a Grail programmer. The fact that Android and iOS
 may suspend tasks and bring them back may necessitate some events to be
 handled but if so the hope is that generic code can be implemented to work
 the same over all platforms.

 1. Finding and eliminating all memory bugs with valgrind and similar tools.

 1. Supporting vector fonts and 3d fonts (as in http://oglft.sourceforge.net/
 https://sourceforge.net/projects/ftgl/)

 1. Heat Map component. Given arrays of x, y, and a separate array of values v,
 write a C++ component and corresponding shader to fill in colors mapped to
 the values. This is analogous to pcolor(x,y) and surf(x,y,z) in MATLAB.

 1. Support for Webrtc https://webrtc.org/ to provide realtime video.

 1. Selection/picking for objects.

 1. Example implementing a JNI interface to Java, or perhaps a sockets
 connection to take a text-only program and turn it into a GUI using Grail as
 an external engine. Generate other language code such as Java from XDL.

 1. Implement menus, buttons, lots of widgets.

 1. Creating a Rust interface to Grail. Alternatively at some point, porting
 Grail to rust.

# list below here must be merged into list above

threaded universal list accepting input from both this machine and other grails using sockets as well as receiving messages via sockets
* add thread to read in mouse and keyboard input, inject into thread-safe list
* accept input from a socket into same thread-safe list
* main wait loop waits on list to draw
* to animate, send message at desired hz. to wake up main wait loop. Is there any way to do this in a continuous loop without the queue?
* key/mouse event map to bind to functions
* gui widgets: button, scrollbar, etc
* wait on a socket for incoming XDL messages

object-oriented cleanup
* refactor graphs to have a common set of features, less code to set any one, reduction in total codebase
* move graph code out of opengl to directory graphs
* find bugs in GapMinder (and move to graphs from opengl)


Data Structures
* modify the Hashmap and make it serializable from a blockloader
* integrate the hashmap into a map so that names can be added to maps

Map
* tessellate map to produce solid color map for non-convex shapes
* remove the redundant points from maps


OpenGL
* heat map vertex and fragment shaders to create an efficient heatmap object
* make it easier to manipulate projections and animation
* add orientation to extrusion operation in CAD
* cad drawing object that renders everyone in cad
* group object that applies a projection and renders everyone inside

data size reduction
* In order to reduce the size of the grail github remove ASCII and large binary data and put in GrailData repository. Data should be acquired separately from the code.

robustness
* print out nice message for each thrown exception
* support nice messages in more than one language
* build in default files into executable and if file is not found, don't load (config? shaders, fonts?)
* preload the fonts, generate the image, and hardcode into executable as well

demos
* client/server demo comparing speed of downloading ASCII data (and parsing) vs. incrementally downloading binary data and updating for stock quotes.
* graphing demo that reads from server rendering the stock quotes above.
* demo with multiple stock quotes simultaneously updating.
* Johns Hopkins style COVID map.
* Solar system simulation in 3d configurable from input file

integration
* add in grail compiled web files from original grail
* add in parser to read markup language and generate compiled grail files

xdl
* default display list<object> on server in tabular form on client
* default display object on server as name-value form

object-oriented refactoring
combine all graphics objects into a single unified family sharing attributes and simplifying setup
combine calendar and other widgets into the same hierarchy
make packages in src/ match the new organization in test

debugging
fix memory bugs and crashing in GapMinder

benchmarking
write client/server that loads a stock price (like AAPL) and sends a window of 400 readings in ASCII going through the whole file. compare that to sending 1 at a time and updating the remote window, sending in binary.

Text
* document class that renders different font sizes, same color
* new shader and document class that renders different colors and embedded images?
* document class that can display a language like Greek encoding the letters as bytes relative to a base * offset in unicode.
* drawing primitives that can display hebrew, arabic right to left
* rectangular formatting like html
equation formatter supporting typical math, superscript, subscript, matrices, etc.


simple graphical tasks
* draw rectangular grid (done?)
* draw button
* draw triangle grid
* draw hex grid
* chessboard
