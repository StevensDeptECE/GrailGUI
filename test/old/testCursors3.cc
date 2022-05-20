#include "grail/Grail.hh"
#include "ActionListener.hh"
#include "opengl/Cursor.hh"
#include <iostream>

using namespace std;

ActionListener *a;
Grail *g;

void mouse_clicked_callback(ActionListener::MouseEvent e) {
  static int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  static Canvas *c = g->cw->getMainCanvas();
  static MultiShape2D *ms = c->addLayer(new MultiShape2D(g->getDefaultStyle()));

  switch (e.type) {
  case ActionListener::MouseType::PRESSED:
    switch (e.data) {
    case ActionListener::MouseData::LEFT:
      x1 = Cursor::getXPos(0);
      y1 = Cursor::getYPos(0);
      break;
    }
    break;
  case ActionListener::MouseType::RELEASED:
    switch (e.data) {
    case ActionListener::MouseData::LEFT:
      x2 = Cursor::getXPos(0);
      y2 = Cursor::getYPos(0);
      ms->drawRectangle(x1, y1, x2-x1, y2-y1);
      c->init();
      cout << "Drawing rectangle at: (" << x1 << ", " << y1 << "), (" << x2 << ", " << y2 << ")" << endl;
      break;
    }
    break;
  }
}

void init(Grail *g) {
  ::g = g;
  g->addDefaultCursor();
  a = new ActionListener(g->cww->inp);
  a->registerCallback(ActionListener::Events::MOUSE, 0, mouse_clicked_callback);
}

int main(int argc, char *argv[]) {
  try {
    Grail::init(1024, 1024, init, nullptr, "out.cml");
    delete a;
  } catch (const char* msg) {
    cerr << msg << endl;
  }
  return 0;
}
