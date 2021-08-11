#include "opengl/GrailGUI.hh"
#include "util/Ex.hh"

using namespace std;
using namespace grail;

void grailmain(int argc, char* argv[], GLWin* w) {
  w->setTitle("Test StyledMultiShape");
  w->setSize(1024,800);
  MainCanvas* c = w->getMainCanvas();
  StyledMultiShape2D* gui = c->getGui();

  const float boxSize = 100;
  const float drawSize = 90;

  gui->fillRectangle(500, 600, 200, 200, red);
  gui->fillRectangle(550, 650, 200, 200, glm::vec4(0, 0, 0, 0.5f));
  gui->drawRectangle(0, 400, drawSize, drawSize, green);
  gui->drawPolygon(boxSize * 1, 400, drawSize, drawSize, 5, blue);
  gui->drawPolygon(boxSize * 2, 400, drawSize, drawSize, 6, blue);
  gui->drawRoundRect(boxSize * 3, 400, drawSize, drawSize, 20, 20, red);
  gui->drawCircle(boxSize * 4.5, boxSize * 0.5, drawSize / 2, 5, purple);
  gui->drawEllipse(boxSize * 5.5, boxSize * 0.5, drawSize / 2, drawSize * .7,
                   5, purple);
  gui->drawLine(boxSize * 6, 0, boxSize * 8, boxSize, green);
  //	gui->drawCompletePolygon(boxSize*7,0, drawSize, drawSize, 6, purple);

  gui->rectanglePoints(100, 100, 200, 300, green);
  gui->trianglePoints(100, 100, 200, 250, 600, 100, green);
  gui->polygonPoints(boxSize * 1, 0, drawSize, drawSize, 5, green);

  float xy[] = {200, 100, 300, 100, 400, 500, 500, 500};
  gui->drawPolyline(xy, 4, red);
  float xy2[] = {400, 100, 550, 120, 650, 100, 550, 220, 400, 205};
  gui->fillPolygon(xy2, 5, cyan);
  gui->drawPolygon(xy2, 5, pink);
  gui->fillRectangle(0, boxSize, drawSize, drawSize, gray);
  gui->fillPolygon(boxSize * 1, boxSize, drawSize, drawSize, 5, yellow);
  gui->fillPolygon(boxSize * 2, boxSize, drawSize, drawSize, 6, yellow);

  gui->fillRoundRect(boxSize * 3, boxSize, drawSize, drawSize, 20, 20, gray);
  gui->fillCircle(boxSize * 7.5, boxSize * 1.5, drawSize / 2, 5, black);
  gui->fillEllipse(boxSize * 8.5, boxSize * 1.5, drawSize / 2, drawSize * .7,
                   5, purple);

  //    gui->bezierSegmentByPoints(0,0, 100,50, 400,300, 400,350,  10, true);
  vector<double> points = {600, 250, 250, 550, 250, 250, 600, 550};
  gui->spline(points, 100, purple);
}

#if 0
Tab* t = w->addTab(); // create a tab and put graphics in it
void switchTab(Tab* t) {}
Tab* t = link(url, request_num); // create a tab and display the data from xdl in it
linkInThisTab(url, request_num); // replace this tab by the results of the link
#endif


int main(int argc, char* argv[]) { 
  try {
    GLWin w(1024, 800, 0xFFFFFFFF, 0x000000FF, "Grail Window");
    w.startWindow();
    w.exitAfter = false;
    grailmain(argc, argv, &w);
    // g->t = thread(crun, g);
    w.mainLoop();
// TODO: move this to GLWin::cleanup or destructor?  FontFace::emptyFaces();
    return 0;
  } catch (const Ex &e) {
    cerr << e << '\n';
  } catch (const char *msg) {
    cerr << msg << endl;
  } catch (const std::exception &e) {
    cerr << e.what() << endl;
  } catch (...) {
    cerr << "uncaught exception! (ouch)\n";
  }
  return 1; // if exception caught return error
}
