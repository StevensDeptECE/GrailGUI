#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
 public:
  const char* mapname;
  TestMultiShape(const char* mapname)
      : GLWin(0x000000, 0xCCCCCC, "Map Demo"), mapname(mapname) {}

#define COLS 2  // Number of columns in data

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    gui->fillRectangle(100, 100, 100, 100, black);

    fstream file;
    vector<vector<float>> shapeVector;
    const float bigFloat = 1E20;
    float yMax = -bigFloat;
    float xMax = -bigFloat;
    float yMin = bigFloat;
    float xMin = bigFloat;
    // Read file
    file.open(mapname, ios::in);  // Open file
    if (file.is_open()) {         // If file has correctly opened...
      // Output debug message
      cout << "File correctly opened" << endl;

      string temp;
      float x, y;
      // Dynamically store data into array
      while (file.peek() != EOF) {  // ... and while there are no errors,
        getline(file, temp, ';');
        shapeVector.push_back(vector<float>());
        vector<float>& points = shapeVector.back();
        stringstream ss(temp);
        while (ss >> x >> y) {
          points.push_back(x);
          points.push_back(y);
          if (x > xMax) xMax = x;
          if (y > yMax) yMax = y;
          if (x < xMin) xMin = x;
          if (y < yMin) yMin = y;
        }
      }
    } else
      cout << "Unable to open file" << endl;
    file.close();

    // cout << xMax << " " << yMax << " " << xMin << " " << yMin << endl;

    double xSize = 1920;
    double ySize = 1080;

    double sX = (xSize - 1) / (xMax - xMin);
    double aX = (xSize - 1) * (-xMin) / (xMax - xMin);

    double sY = (ySize - 1) / (yMax - yMin);
    double aY = ((ySize - 1) * (-yMin) / (yMax - yMin));

    for (int i = 0; i < shapeVector.size(); i++) {
      for (int j = 0; j < shapeVector[i].size(); j += 2) {
        shapeVector[i][j] = sX * shapeVector[i][j] + aX;
        shapeVector[i][j + 1] = 1080 - (sY * shapeVector[i][j + 1] + aY);
        // shapeVector[i][j] = (xSize*(shapeVector[i][j]/xMax));
        // shapeVector[i][j+1] = (ySize+2200) -
        // (3*ySize*(shapeVector[i][j+1]/yMax));
        if (shapeVector[i][j] < 0 || shapeVector[i][j + 1] < 0)
          cout << shapeVector[i][j] << " " << shapeVector[i][j + 1] << endl;
      }
      gui->drawPolygon(shapeVector[i], black);
      // for(int j = 0; j < shapeVector[i].size(); j+=2) cout <<
      // shapeVector[i][j] << " " << shapeVector[i][j+1] << endl;
    }
  }
};

int main(int argc, char* argv[]) {
  const char* mapname = argc < 2 ? "shapefiles/Counties.txt" : argv[1];
  return GLWin::init(new TestMultiShape(mapname), 1920, 1080);
}
