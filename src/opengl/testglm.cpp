#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;
ostream& operator <<(ostream& s, const glm::mat4& m) {
  return s
   << m[0][0] << '\t' << m[0][1] << '\t' << m[0][2] << '\t' << m[0][3] << \n' <<
   << m[1][0] << '\t' << m[1][1] << '\t' << m[1][2] << '\t' << m[1][3] << '\n' <<
   << m[2][0] << '\t' << m[2][1] << '\t' << m[2][2] << '\t' << m[2][3] << '\n' <<
   << m[3][0] << '\t' << m[3][1] << '\t' << m[3][2] << '\t' << m[3][3] << "\n\n";
}
 
int main() {
  glm::mat4 a(1);
  glm::translate(a, vec3(1,0,0));
  glm::rotate(a, 45*numbers::pi/180, vec3(1,0,0));
  cout << a;
}