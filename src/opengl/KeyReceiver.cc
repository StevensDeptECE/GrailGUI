#include "opengl/KeyReceiver.hh"

#include "opengl/Canvas.hh"

KeyReceiver::KeyReceiver(Canvas* c) { c->getWin()->addKeyReceiver(this); }
