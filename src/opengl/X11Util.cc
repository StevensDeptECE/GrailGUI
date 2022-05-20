#include "X11Util.hh"

#include <X11/X.h>
#include <X11/Xlib.h>

static Display *dpy = XOpenDisplay(0);
static Window root_window = XRootWindow(dpy, 0);

void X11Util::setCursor(float w_x, float w_y, float dx, float dy) {
  XSelectInput(dpy, root_window, KeyReleaseMask);
  XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, w_x + dx, w_y + dy);
  XFlush(dpy);
}
