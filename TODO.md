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
  and elegant. The current codebase is approximately 15k lines, 458kb.
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
 that is manageable and understandable.
 
## Portability

 GrailGUI is currently implemented on Linux (tested on Ubuntu and Centos)
 and Windows (at the time of writing, the sockets code has not yet been ported)
 We are currently porting to Raspberry Pi.

## TODO

 The following items are high on the TODO list. If you are a capable OpenGL
 programmer and wish to contribute, we would love to hear from you:

 1. Porting to Mac OSX, Android, iphone. The critical thing is to make this
 absolutely invisible to the application programmer.

 2. Supporting vector fonts and 3d fonts (as in http://oglft.sourceforge.net/
 https://sourceforge.net/projects/ftgl/)

 3. Heat Map component. Given arrays of x, y, and a separate array of values v,
 write a C++ component and corresponding shader to draw

 4. Support for Webrtc https://webrtc.org/

 5. Selection/picking for objects.

 6. Example implementing a JNI interface to Java, or perhaps a sockets
 connection to take a text-only program and turn it into a GUI using Grail as
 an external engine. Generate other language code such as Java from XDL.

 7. Implement menus, buttons, lots of widgets.