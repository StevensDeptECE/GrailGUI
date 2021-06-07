#include "opengl/GrailGUI.hh"
#include"opengl/ButtonWidget.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
public:
	TestMultiShape() :
		GLWin(0x000000, 0xCCCCCC, "Map Demo") {}

#define FILENAME "shapefiles/Counties.txt"
#define COLS 2 // Number of columns in data


	void init() {
		MainCanvas* c = currentTab()->getMainCanvas();
		StyledMultiShape2D* gui = c->getGui();
		
    gui->fillRectangle(100, 100, 100, 100, black);

		fstream file;
		vector <vector <float>> shapeVector;
		float yMax = 0;
		float xMax = 0;

		// Read file
		file.open(FILENAME, ios::in); // Open file
		if (file.is_open()) { // If file has correctly opened...
			// Output debug message
			cout << "File correctly opened" << endl;

			// Dynamically store data into array
			while (file.peek()!=EOF) { // ... and while there are no errors,
				string temp;
				getline(file,temp,';');
				stringstream ss(temp);
				for(int i = 0; ss; i++){
        	for(int k = 0; ss; k++){
						shapeVector.push_back(vector<float>());
						for(int j = 0; j < COLS; j++){
							string s;
							getline(ss,s,',');
							int temp = abs(atof(s.c_str()));
							//cout << temp << endl;
							if(j%2 == 1){
								if(temp > xMax){
									xMax = temp;
									
									//cout << s << endl;
									//cout << temp << " " << j << endl << endl;
								}
							}else{
								if(temp > yMax){
									//cout << s << endl;
									//cout << temp << " " << j << endl << endl;
									yMax = temp;
								}
							}
							shapeVector[i].push_back(temp);
							//cout << shapeVector[i][k] << endl;
						}
					}
				}
			}
		}

		else cout << "Unable to open file" << endl;
		file.close();


		double xSize = 1920;
		double ySize = 1080;
		int start = 1;

		cout << xMax << " " << yMax << endl;
	/*
		for(int i = 0; i < 1;i++){
			double max = *max_element(shapeVector[i].begin(), shapeVector[i].end());
			for(int k = start;k < shapeVector[i].size()/2;k+=2){
				//cout << shapeVector[i][k] << " " << shapeVector[i][k+1] << " " << yMax << endl;
				//cout << shapeVector[i][k] << endl;
				shapeVector[i][k] = ( xSize * (shapeVector[i][k] / xMax));
				//cout << shapeVector[i][k] << endl << endl;
				shapeVector[i][k+1] = ySize - ( ySize * (shapeVector[i][k+1] / yMax));
				//cout << shapeVector[i][k] << " " << shapeVector[i][k+1] << endl << endl;
				//cout << abs(shapeVector[i][k-2] - shapeVector[i][k]) << " " << shapeVector[i][k-2] << " " << shapeVector[i][k-1] << endl;
				if(abs(shapeVector[i][k-2] - shapeVector[i][k]) < 1000 && shapeVector[i][k-2] > 0 && shapeVector[i][k-1] > 0){
        	gui->drawLine(shapeVector[i][k-2],shapeVector[i][k-1],shapeVector[i][k],shapeVector[i][k+1],black); //TODO: Draw As Polygons
				  cout << shapeVector[i][k-2] << "," << shapeVector[i][k-1] << " to " << shapeVector[i][k] << "," << shapeVector[i][k+1] << " Distance : "<< abs(shapeVector[i][k-2] - shapeVector[i][k]) << " Num : " << k <<  endl << endl;
				}
			}		
		}
*/
		for(int i = 0; i < 1; i++){
			for(int j = start; j < shapeVector[i].size()/2;j+=2){
				shapeVector[i][j] = ( xSize * (shapeVector[i][j] / xMax));
				shapeVector[i][j+1] = ySize - ( ySize * (shapeVector[i][j+1] / yMax));
			}
			gui->drawPolygon(shapeVector[i],black); //TODO: Crashes, probably sending too big a vector
		}

	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new TestMultiShape(),1920,1080);
}
