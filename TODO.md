# Main Goals of GrailGUI Project

## Overview

 GrailGUI is set of C++ classes providing a simple wrapper for OpenGL with
 a standard set of objects and shaders that currently renders:
   1. 2D shapes
   2. 2D text,
   3. Images
   4. Support layers of objects (2.5D)
   5. Portable GUI widgets using OpenGL primitives
   6. 3D objects with textures


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
   2. Ability to save screenshots
   3. Ability to save a movie (though very slow)
   4. Predefined move the camera
   5. Configuration for internationalization
   6. Network connection to servers using high-speed binary data using XDL,
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
   2. Extensible components
   
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

 2. Finding and eliminating all memory bugs with valgrind and similar tools.

 3. Supporting vector fonts and 3d fonts (as in http://oglft.sourceforge.net/
 https://sourceforge.net/projects/ftgl/)

 4. Heat Map component. Given arrays of x, y, and a separate array of values v,
 write a C++ component and corresponding shader to fill in colors mapped to
 the values. This is analogous to pcolor(x,y) and surf(x,y,z) in MATLAB.

 5. Support for Webrtc https://webrtc.org/ to provide realtime video.

 6. Selection/picking for objects.

 7. Example implementing a JNI interface to Java, or perhaps a sockets
 connection to take a text-only program and turn it into a GUI using Grail as
 an external engine. Generate other language code such as Java from XDL.

 8. Implement menus, buttons, lots of widgets.

 9. Creating a Rust interface to Grail. Alternatively at some point, porting
 Grail to rust.
