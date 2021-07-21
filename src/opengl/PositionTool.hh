#pragma once
#include <vector>

#include "opengl/Widget2D.hh"
using namespace std;


class PositionTool : public Widget2D {
 private:
  vector<float> xCord;
  vector<float> yCord;
  vector<float> radius;

 public:
  PositionTool(StyledMultiShape2D* m, MultiText* t, float x, float y, float w,
              float h, vector<float>& X,vector<float>& Y, vector<float>& r)
      : Widget2D(m, t, x, y, w, h), xCord(X),yCord(Y),radius(r) {
    
    }

  void addNewCord (float newX, float newY ,float newR);

  void update(float newX, float newY, uint32_t position);

  void init() override;

};
