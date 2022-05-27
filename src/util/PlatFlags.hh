
// Windows uses O_BINARY and O_TEXT to differentiate between binary and text in
// read and write operations. This allows us to use the flags without them
// having any effects
#ifndef _WIN32
#define O_BINARY 0x0
#define O_TEXT 0x0
#endif
