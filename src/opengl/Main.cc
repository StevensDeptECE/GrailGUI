#include <iostream>

#include "opengl/GrailGUI.hh"

using namespace std;

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab);

int main(int argc, char* argv[]) {
  try {
    GLWin w(1024, 800, 0xFFFFFFFF, 0x000000FF, "Grail Window");
    Tab* tab = w.currentTab();
    grailmain(argc, argv, &w, tab);
    w.mainLoop();
    // g->t = thread(crun, g);
    // TODO: move this to GLWin::cleanup or destructor?  FontFace::emptyFaces();
    return 0;
  } catch (const Ex& e) {
    cerr << e << '\n';
  } catch (const char* msg) {
    cerr << msg << endl;
  } catch (const std::exception& e) {
    cerr << e.what() << endl;
  } catch (...) {
    cerr << "uncaught exception! (ouch)\n";
  }
  return 1;  // if exception caught return error
}