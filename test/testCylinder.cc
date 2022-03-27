#include <unistd.h>

#include "CAD/Cylinder.hh"
using namespace std;
using namespace grail;
int main(){
    Cylinder cylinder(2, 2, 2);
    double volume = cylinder.getVolume();
    cout << "Volume of Cylinder: " << volume << "\n";
    cout << "Expected 25.13\n";

    Cylinder cylinder(2, 2);
    double volume = cylinder.getVolume();
    cout << "Volume of Cylinder: " << volume << "\n";
    cout << "Expected 25.13\n";
}