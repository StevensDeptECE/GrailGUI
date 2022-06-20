#include <iostream>

#include "opengl/GrailGUI.hh"
#include "util/Benchmark.hh"
#include "util/Prefs.hh"

using namespace std;
using namespace grail::utils;

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab);

int main(int argc, char* argv[]) {
  try {
    CBenchmark<> b1("Grail Create GLWin");

    b1.start();
    GLWin w(1024, 800, 0xFFFFFFFF, 0x000000FF, "Grail Window");
    b1.endAndDisplay(prefs.shouldDisplay(LogLevel::INFO)); // How long to create the window ? 
    Tab* tab = w.currentTab();

    CBenchmark<> b2("Grail run grailmain");
    b2.start();
    grailmain(argc, argv, &w, tab);
    b2.endAndDisplay(prefs.shouldDisplay(LogLevel::INFO));  // How long to run grailmain?

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