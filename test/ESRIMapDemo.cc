#include <algorithm>
#include <chrono>

#include "opengl/GrailGUI.hh"
#include "opengl/Shapefile.hh"

#define curtime chrono::high_resolution_clock::now();

using namespace std;
using namespace grail;

class ESRIMapLoader : public GLWin {
 private:
  vector<vector<float>> flatvec;

 public:
  ESRIMapLoader() : GLWin(0x000000, 0xCCCCCC, "ESRI Shapefile Map Demo") {}
  int countyStart;
  int numCounties;
  int displayNumCounties;
  int ld_countyStart;
  int ld_displayNumCounties;
  time_t actionTimer;
  bool animate;
  //chrono::time_point < chrono::high_resolution_clock timer = ;

  //double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();

  static void
  nextCounty(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    //if (difftime(stdtime(nullptr), eml->actionTimer) >)
    if (eml->countyStart < eml->numCounties)
      eml->countyStart++;
    //eml->
  }
  static void prevCounty(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->countyStart > 0)
      eml->countyStart--;
  }
  static void displayAllCounties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    eml->countyStart = 0;
    eml->displayNumCounties = eml->numCounties;
  }
  static void display3Counties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    eml->displayNumCounties = 3;
  }
  static void decreaseCounties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->displayNumCounties > 0)
      eml->displayNumCounties--;
  }
  static void increaseCounties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->displayNumCounties < eml->numCounties)
      eml->displayNumCounties++;
  }

  static void toggleAnimate(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    eml->animate = !eml->animate;
  }

  void detectCycle(vector<float> vec) {
    vector<pair<float, float>> indices(vec.size() / 2 + 1);
    indices[0] = pair(-1, -1);
    pair<float, float> temp;
    vector<pair<float, float>>::iterator it;
    cout << vec.size() << "\n";
    for (int i = 0; i < vec.size(); i += 2) {
      temp = pair(vec[i], vec[i + 1]);
      it = find(indices.begin(), indices.end(), temp);
      if (it != indices.end()) {
        cout << "Cycle detected?\n"
             << "Point found: " << temp.first << " " << temp.second << "\n"
             << "First point: " << vec[0] << " " << vec[1] << "\n";
      }
    }
    cout << vec[0] << " " << vec[1] << "\n"
         << vec[vec.size() - 2] << " " << vec[vec.size() - 1] << "\n";
  }

  void transform(ESRIPoint& pt, double shiftX, double shiftY, double scaleX, double scaleY) {
    pt.x = pt.x * scaleX + shiftX;
    pt.y = pt.y * scaleY + shiftY;
  }

  vector<float> flatten(vector<ESRIPoint>& vec) {
    vector<float> flatvec(vec.size() * 2);
    for (int i = 0, j = 0; i < vec.size(); i++, j += 2) {
      flatvec[j] = (float)vec[i].x;
      flatvec[j + 1] = (float)vec[i].y;
    }
    return flatvec;
  }

  void init() {
    Shapefile counties = Shapefile("res/us_counties/USA_Counties.shp");
    counties.init();

    // Convert shape objects to our ESRIShape class
    vector<unique_ptr<ESRIShape>> shapes = ESRIShape::convertSHPObjects(counties.getShapeVector());

    // Set display parameters
    countyStart = 0;
    numCounties = shapes.size();
    displayNumCounties = 3;
    ld_countyStart = -1;
    ld_displayNumCounties = -1;
    //timer = curtime;
    //actionTimer = curtime;

    // Get counties from ESRIShapes
    vector<vector<ESRIPoint>> shapeVector(numCounties);
    for (int i = 0; i < shapes.size(); i++) {
      shapeVector[i] = shapes[i]->getPoints();
    }

    // Set window and transform properties
    int xSize = 1920;
    int ySize = 1080;

    double* maxBounds = counties.getMaxBounds();
    double* minBounds = counties.getMinBounds();
    double shiftX = 1300 - minBounds[0] * xSize / (maxBounds[0] - minBounds[0]);
    double shiftY = 700 + ySize - (minBounds[1] * ySize / (maxBounds[1] - minBounds[1]));
    double scaleX = 2.5 * xSize / (maxBounds[0] - minBounds[0]);
    double scaleY = -ySize / (maxBounds[1] - minBounds[1]);

    // Keybinds for animation!
    setAction(1002, displayAllCounties);
    setAction(1003, display3Counties);
    setAction(1004, nextCounty);
    setAction(1005, prevCounty);
    setAction(1006, increaseCounties);
    setAction(1007, decreaseCounties);
    setAction(1008, toggleAnimate);

    setEvent(266, 1002);  // page up = displayAll
    setEvent(267, 1003);  // page down = display3
    setEvent(262, 1004);  // right arrow = nextCount
    setEvent(263, 1005);  // left arrow = prevCounty
    setEvent(265, 1006);  // up arrow = increase  display
    setEvent(264, 1007);  // down arrow = decrease display
    setEvent(46, 1008);   // period = toggle animation

    //double xMin = -80, xMax = -60;
    //for (auto& shape : shapeVector) {
    //int j = 0;
    //for (int i = 0; i < shape.size(); i++) {
    //if (shape[i].x >= xMin && shape[i].x <= xMax) {
    //shape[j++] = shape[i];
    //}
    //}
    //shape.resize(j);
    //}
    flatvec.reserve(shapeVector.size());
    for (auto& shape : shapeVector) {
      for (auto& point : shape) {
        transform(point, shiftX, shiftY, scaleX, scaleY);
      }
      flatvec.push_back(flatten(shape));
      //detectCycle(flatvec.back());
    }
    update();
  }

  void update() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    if (ld_countyStart != countyStart ||
        ld_displayNumCounties != displayNumCounties) {
      gui->clear();
      int bound = countyStart + displayNumCounties;
      bound = (bound > numCounties) ? numCounties : bound;
      cout << countyStart << " " << bound << "\n";
      for (int i = countyStart; i < bound; i++) {
        gui->drawPolygon(flatvec[i], black);
      }
      gui->updatePoints();
      gui->updateIndices();
      setDirty();
      ld_displayNumCounties = displayNumCounties;
      ld_countyStart = countyStart;
    }

    //chrono::duration<double, std::milli>(timer -).count();
    /*if (animate && (&timer == nullptr ||
                    dt > 1.5)) {
      //timer = stdtime(&timer);
      countyStart = (countyStart >= numCounties) ? 0 : ++countyStart;
    }*/
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new ESRIMapLoader(), 1920, 800);
}
