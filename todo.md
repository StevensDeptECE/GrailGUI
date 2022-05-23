# This is a global list of all features suggested for Grail.
# From this list we will prioritize and implement the most urgent.

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
* find bugs in GapMinder


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

data size reduction
* In order to reduce the size of the grail github remove ASCII and large binary data and put in GrailData repository. Data should be acquired separately from the code.

robustness
* print out nice message for each thrown exception
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



simple graphical tasks
draw rectangular grid
draw button
draw triangle grid
draw hex grid
chessboard
