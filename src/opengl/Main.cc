#include <iostream>

#include "opengl/GrailGUI.hh"
#include "util/Benchmark.hh"
#include "util/Prefs.hh"

using namespace std;
using namespace grail::utils;

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab);

void mainErrorHandling(const char title[], uint32_t width, uint32_t height,
  uint32_t fgColor, uint32_t bgColor, int argc, char* argv[]) {
  try {
    CBenchmark<> b1("Grail Create GLWin");

    b1.start();
    GLWin w(width, height, fgColor, bgColor, title);
    b1.endAndDisplay(prefs.shouldDisplay(LogLevel::INFO)); // How long to create the window ? 
    Tab* tab = w.currentTab();

    CBenchmark<> b2("Grail run grailmain");
    b2.start();
    tab->loadBindings();
    grailmain(argc, argv, &w, tab);
    b2.endAndDisplay(prefs.shouldDisplay(LogLevel::INFO));  // How long to run grailmain?

    w.mainLoop();
    // g->t = thread(crun, g);
    // TODO: move this to GLWin::cleanup or destructor?  FontFace::emptyFaces();
    exit(0);
  } catch (const Ex& e) {
    cerr << e << '\n';
  } catch (const char* msg) {
    cerr << msg << endl;
  } catch (const std::exception& e) {
    cerr << e.what() << endl;
  } catch (...) {
    cerr << "uncaught exception! (ouch)\n";
  }
  exit(1);  // if exception caught return error
}

#if 0
int main(int argc, char* argv[]) {
  mainErrorHandling("Grail Window", 1024, 800, 0xFFFFFFFF, 0x000000FF, argc, argv);
  return 0;
}
#endif
