#include <algorithm>
#include <chrono>

#include "opengl/GrailGUI.hh"
#include "opengl/Shapefile.hh"

using namespace std;
using namespace grail;

#define curTime chrono::high_resolution_clock::now()
#define milliCurTime chrono::time_point_cast<chrono::milliseconds>(curTime)
#define milliDuration chrono::duration<double, std::milli>

typedef chrono::time_point<chrono::system_clock, chrono::milliseconds> hrctime;

class Timer {
 public:
  Timer() : beg_(clock_::now()) {}
  void reset() { beg_ = clock_::now(); }
  double elapsed() const {
    return std::chrono::duration_cast<ms_>(clock_::now() - beg_).count();
  }

 private:
  typedef std::chrono::high_resolution_clock clock_;
  typedef std::chrono::duration<double, std::milli> ms_;
  std::chrono::time_point<clock_> beg_;
};

class ESRIMapLoader : public GLWin {
 private:
  vector<vector<float>> flatvec;
  int ld_countyStart;
  int ld_displayNumCounties;
  Timer animateTimer;
  uint64_t animateDelay;

 public:
  ESRIMapLoader() : GLWin(0x000000, 0xCCCCCC, "ESRI Shapefile Map Demo") {}
  int countyStart;
  int numCounties;
  int displayNumCounties;
  Timer actionTimer;
  uint64_t actionDelay = 100;
  bool animate;

  static void nextCounty(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      cerr << "action time:" << eml->actionTimer.elapsed();
      if (eml->countyStart < eml->numCounties) eml->countyStart++;
      eml->actionTimer.reset();
    }
  }

  static void prevCounty(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      if (eml->countyStart > 0) eml->countyStart--;
      eml->actionTimer.reset();
    }
  }
  static void displayAllCounties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      eml->countyStart = 0;
      eml->displayNumCounties = eml->numCounties;
      eml->actionTimer.reset();
    }
  }
  static void display3Counties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      eml->displayNumCounties = 3;
      eml->actionTimer.reset();
    }
  }
  static void decreaseCounties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      if (eml->displayNumCounties > 0) eml->displayNumCounties--;
      eml->actionTimer.reset();
    }
  }
  static void increaseCounties(GLWin* w) {
    ESRIMapLoader* eml = ((ESRIMapLoader*)w);
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      if (eml->displayNumCounties < eml->numCounties) eml->displayNumCounties++;
      eml->actionTimer.reset();
    }
  }

  static void toggleAnimate(GLWin* w) {
    ESRIMapLoader* eml = (ESRIMapLoader*)w;
    if (eml->actionTimer.elapsed() > eml->actionDelay) {
      eml->animate = !eml->animate;
      eml->actionTimer.reset();
    }
  }

  void detectCycle(vector<float> vec) {
    vector<pair<float, float>> indices(vec.size() / 2 + 1);
    indices[0] = pair<float, float>(-1, -1);
    pair<float, float> temp;
    vector<pair<float, float>>::iterator it;
    cout << vec.size() << "\n";
    for (int i = 0; i < vec.size(); i += 2) {
      temp = pair<float, float>(vec[i], vec[i + 1]);
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

  void filterX(vector<vector<ESRIPoint>> shapeVector, double xMin,
               double xMax) {
    for (auto& shape : shapeVector) {
      int j = 0;
      for (int i = 0; i < shape.size(); i++) {
        if (shape[i].x >= xMin && shape[i].x <= xMax) {
          shape[j++] = shape[i];
        }
      }
      shape.resize(j);
    }
  }
  void transform(ESRIPoint& pt, double shiftX, double shiftY, double scaleX,
                 double scaleY) {
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
    vector<unique_ptr<ESRIShape>> shapes =
        ESRIShape::convertSHPObjects(counties.getShapeVector());

    // Set display parameters
    countyStart = 0;
    numCounties = shapes.size();
    displayNumCounties = 3;
    ld_countyStart = -1;
    ld_displayNumCounties = -1;

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
    double shiftY =
        400 + ySize - (minBounds[1] * ySize / (maxBounds[1] - minBounds[1]));
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
    setAction(1009, zoomIn2D);
    setAction(1010, zoomOut2D);
    setAction(1011, panLeft2D);
    setAction(1012, panRight2D);
    setAction(1013, panUp2D);
    setAction(1014, panDown2D);
    setAction(1015, resetProjection2D);

    setEvent(266, 1002);  // page up = displayAll
    setEvent(267, 1003);  // page down = display3
    setEvent(262, 1004);  // right arrow = nextCount
    setEvent(263, 1005);  // left arrow = prevCounty
    setEvent(265, 1006);  // up arrow = increase  display
    setEvent(264, 1007);  // down arrow = decrease display
    setEvent(46, 1008);   // period = toggle animation
    setEvent(401, 1009);  // scroll up zoom in 2d
    setEvent(399, 1010);  // scroll down zoom in 2d
    setEvent('W', 1013);  // w PAN up
    setEvent('S', 1014);  // S PAN Down
    setEvent('A', 1011);  // c pan left
    setEvent('D', 1012);  // d pan right

    setEvent('Q', 1009);  // zoom in
    setEvent('E', 1010);  // zoom out
    setEvent('P', 1015);
    double xMin = -80, xMax = -60;
    filterX(shapeVector, xMin, xMax);
    flatvec.reserve(shapeVector.size());
    for (auto& shape : shapeVector) {
      for (auto& point : shape) {
        transform(point, shiftX, shiftY, scaleX, scaleY);
      }
      flatvec.push_back(flatten(shape));
      // detectCycle(flatvec.back());
    }
    animateDelay = 1000 / log2(flatvec.size());
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

    if (animate && animateTimer.elapsed() > animateDelay) {
      countyStart = (countyStart >= numCounties) ? 0 : ++countyStart;
      animateTimer.reset();
    }
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new ESRIMapLoader(), 1920, 1500);
}
