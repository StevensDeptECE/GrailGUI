#include <unistd.h>

#include "CAD/RectangularPrism.hh"
using namespace std;
using namespace grail;
int main(){
    RectangularPrism rectangularPrism(3, 4, 5);
    double volume = rectangularPrism.getVolume();
    cout << "Volume of Rectangular Prism: " << volume << "\n";
    cout << "Expected 60\n";
}