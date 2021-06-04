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

#define FILENAME "Cities_Towns.txt"
#define COLS 2 // Number of columns in data


	void init() {
		MainCanvas* c = currentTab()->getMainCanvas();
		StyledMultiShape2D* gui = c->getGui();

    


	fstream file;
	vector <vector <vector <float>>> shapeVector;

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
					shapeVector.push_back(vector<vector<float>>());
					shapeVector[i].push_back(vector<float>());
					shapeVector[i].push_back(vector<float>());
					for(int j = 0; j < COLS; j++){
						string s;
						getline(ss,s,',');
						//cout << s << endl;
						shapeVector[i][j].push_back(abs(atof(s.c_str())));
					}
				}
			}
		}
	}

	else cout << "Unable to open file" << endl;
	file.close();


	int xSize = 1920;
	int ySize = 1080;
	int start = 1;

	for(int i = 0; i < 1;i++){
			double xMax = *max_element(shapeVector[i][0].begin(), shapeVector[i][0].end());
			double yMax = *max_element(shapeVector[i][1].begin(), shapeVector[i][1].end());
			//cout << yMax;
			for(int k = start;k < shapeVector[i][0].size();k++){
				//cout << shapeVector[i][0][k] << " " << shapeVector[i][1][k] << " " << xMax << endl;
				shapeVector[i][0][k] = xSize - ((xSize*shapeVector[i][0][k])/xMax);
				shapeVector[i][1][k] = ySize - ((ySize*shapeVector[i][1][k])/yMax);
				//cout << shapeVector[i][0][k] << " " << shapeVector[i][1][k] << endl << endl;
				if(abs(shapeVector[i][0][k-1] - shapeVector[i][0][k]) < 100 && shapeVector[i][0][k-1] > 0 && shapeVector[i][0][k] > 0){
        	gui->drawLine(shapeVector[i][0][k-1],shapeVector[i][1][k-1],shapeVector[i][0][k],shapeVector[i][1][k],black);
				  //cout << shapeVector[i][0][k-1] << " " << shapeVector[i][0][k] << " " << abs(shapeVector[i][0][k-1] - shapeVector[i][0][k]) << " " << k <<  endl << endl;
				}
			}		
	}

	

	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new TestMultiShape(),1920,1080);
}
