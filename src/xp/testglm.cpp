#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

int main() {
  glm::mat4 a(1);
  glm::translate(a, vec3(1,0,0));
  glm::rotate(a, 45*numbers::pi/180, vec3(1,0,0));
  cout << a;
}