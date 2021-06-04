#include "opengl/GrailGUI.hh"
#include"opengl/ButtonWidget.hh"
//#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <algorithm>
using namespace std;
using namespace grail;

class TestMultiShape : public GLWin {
public:
	TestMultiShape() :
		GLWin(0x000000, 0xCCCCCC, "Test StyledMultiShape") {}

#define FILENAME "output2.txt"
#define COLS 2 // Number of columns in data


	void init() {
		MainCanvas* c = currentTab()->getMainCanvas();
		StyledMultiShape2D* gui = c->getGui();

    

		const float boxSize = 75;
		const float drawSize = (boxSize/4)*5;

    float xy2[] = {boxSize+drawSize/2, drawSize/2, drawSize/2, boxSize+drawSize/2, 7*boxSize+drawSize/2, 5*boxSize+drawSize/2, 8*boxSize+drawSize/2, 4*boxSize+drawSize/2};
		gui->fillPolygon(xy2, 4, blue);
    
    
    gui->fillRectangle(boxSize, boxSize, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize, boxSize, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize, boxSize*2, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*2, boxSize*1.5, drawSize/5, drawSize*.4, black);
    gui->fillRectangle(boxSize*1.5, boxSize*1.5, drawSize*.4, drawSize/5, black);

    gui->fillRectangle(boxSize*2.5, boxSize*1.75, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*2.5, boxSize*1.75, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*2.5, boxSize*2.25, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*3.5, boxSize*1.75, drawSize/5, drawSize*.4, black);
    gui->fillRectangle(boxSize*2.75, boxSize*2.5, drawSize*.4, drawSize/5, black);
    gui->fillRectangle(boxSize*3.25, boxSize*2.75, drawSize*.4, drawSize/5, black);
    

    gui->fillRectangle(boxSize*4, boxSize*2.5, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*4, boxSize*2.5, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*5, boxSize*2.5, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*4, boxSize*3, drawSize, drawSize/5, black);

    gui->fillRectangle(boxSize*5.5, boxSize*3.25, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*5.5, boxSize*4.25, drawSize, drawSize/5, black);
    gui->fillRectangle(boxSize*6, boxSize*3.25, drawSize/5, drawSize, black);
   
    gui->fillRectangle(boxSize*7, boxSize*4, drawSize/5, drawSize, black);
    gui->fillRectangle(boxSize*7, boxSize*5, drawSize, drawSize/5, black);

    glm::vec4 v = {1,2,3,4};
    float w  = 7;

    gui->drawLine(w*31,w*85.002,w*30.997,w*87.598,black);
    gui->drawLine(w*30.997,w*87.598,w*30.846,w*87.625,black);
    gui->drawLine(w*30.846,w*87.625,w*30.655,w*87.397,black);
    gui->drawLine(w*30.655,w*87.397,w*30.466,w*87.426,black);
    //gui->drawLine(w*30.466,w*87.426,w*31,w*85.002,black);
    //gui->drawLine(w*34.995,w*88.2,w*34.984,w*85.605,black);
gui->drawLine(w*34.984,w*85.605,w*32.864,w*85.184,black);
gui->drawLine(w*32.864,w*85.184,w*32.426,w*84.964,black);
gui->drawLine(w*32.426,w*84.964,w*32.326,w*85.006,black);
gui->drawLine(w*32.326,w*85.006,w*32.261,w*84.889,black);
gui->drawLine(w*32.261,w*84.889,w*32.133,w*85.062,black);
gui->drawLine(w*32.133,w*85.062,w*31.84,w*85.141,black);
gui->drawLine(w*31.84,w*85.141,w*31.538,w*85.041,black);
gui->drawLine(w*31.538,w*85.041,w*31.188,w*85.108,black);
gui->drawLine(w*31.188,w*85.108,w*31,w*85.002,black);

gui->drawLine(w*34.995,w*88.2,w*34.984,w*85.605,black);

//gui->drawLine(w*31,w*85.002,w*30.348,w*88.394,black);
gui->drawLine(w*30.348,w*88.394,w*31.894,w*88.473,black);
gui->drawLine(w*31.894,w*88.473,w*32.732,w*88.37,black);
gui->drawLine(w*32.732,w*88.37,w*32.747,w*88.368,black);
gui->drawLine(w*32.747,w*88.368,w*34.892,w*88.097,black);
gui->drawLine(w*34.892,w*88.097,w*34.995,w*88.2,black);
gui->drawLine(w*34.995,w*88.2,w*34.995,w*88.2,black);

////

gui->drawLine(w*33.637,w*94.485,w*33.961,w*95.227,black);
gui->drawLine(w*33.961,w*95.227,w*33.873,w*95.28,black);
gui->drawLine(w*33.873,w*95.28,w*33.939,w*95.6,black);
gui->drawLine(w*33.939,w*95.6,w*33.845,w*95.776,black);
gui->drawLine(w*33.845,w*95.776,w*33.838,w*96.147,black);
gui->drawLine(w*33.838,w*96.147,w*33.687,w*96.352,black);
gui->drawLine(w*33.687,w*96.352,w*33.843,w*96.625,black);
gui->drawLine(w*33.843,w*96.625,w*33.894,w*96.587,black);
gui->drawLine(w*33.894,w*96.587,w*33.823,w*96.758,black);
gui->drawLine(w*33.823,w*96.758,w*33.955,w*96.985,black);
gui->drawLine(w*33.955,w*96.985,w*33.718,w*97.135,black);
gui->drawLine(w*33.718,w*97.135,w*33.914,w*97.218,black);
gui->drawLine(w*33.914,w*97.218,w*33.819,w*97.427,black);
gui->drawLine(w*33.819,w*97.427,w*33.991,w*97.669,black);
gui->drawLine(w*33.991,w*97.669,w*33.856,w*97.84,black);
gui->drawLine(w*33.856,w*97.84,w*33.881,w*97.97,black);
gui->drawLine(w*33.881,w*97.97,w*33.989,w*97.946,black);
gui->drawLine(w*33.989,w*97.946,w*34.011,w*98.091,black);
gui->drawLine(w*34.011,w*98.091,w*34.153,w*98.117,black);
gui->drawLine(w*34.153,w*98.117,w*34.148,w*98.35,black);
gui->drawLine(w*34.148,w*98.35,w*34.057,w*98.458,black);
gui->drawLine(w*34.057,w*98.458,w*34.161,w*98.631,black);
gui->drawLine(w*34.161,w*98.631,w*34.114,w*98.741,black);
gui->drawLine(w*34.114,w*98.741,w*34.213,w*99.186,black);
gui->drawLine(w*34.213,w*99.186,w*34.453,w*99.361,black);
gui->drawLine(w*34.453,w*99.361,w*34.373,w*99.405,black);
gui->drawLine(w*34.373,w*99.405,w*34.381,w*99.71,black);
gui->drawLine(w*34.381,w*99.71,w*34.557,w*99.998,black);
gui->drawLine(w*34.557,w*99.998,w*36.5,w*100.003,black);
gui->drawLine(w*36.5,w*100.003,w*36.5,w*103.001,black);

gui->drawLine(w*36.499,w*94.617,w*35.398,w*94.431,black);
gui->drawLine(w*35.398,w*94.431,w*33.637,w*94.485,black);

gui->drawLine(w*36.998,w*94.618,w*36.499,w*94.617,black);

gui->drawLine(w*36.998,w*94.618,w*36.993,w*102.041,black);


gui->drawLine(w*37,w*103.001,w*36.993,w*102.041,black);

gui->drawLine(w*37,w*103.001,w*36.5,w*103.001,black);

// Variable declarations
	fstream file;
	vector <float> xVals; // 2d array as a vector of vectors
	vector <float> yVals;

	// Read file
	file.open(FILENAME, ios::in); // Open file
	if (file.is_open()) { // If file has correctly opened...
		// Output debug message
		cout << "File correctly opened" << endl;

		// Dynamically store data into array
		while (file.peek()!=EOF) { // ... and while there are no errors,
        string s;
				//cout << s << endl;
        getline(file,s,',');
				xVals.push_back(atof(s.c_str())); // fill the row with col element
        getline(file,s,',');
				yVals.push_back(atof(s.c_str()));
				//cout << array[row][col] << endl << endl;
		}
	}
	else cout << "Unable to open file" << endl;
	file.close();

	int a = 1024;

	double xMax = *max_element(xVals.begin(), xVals.end());
	double yMax = *max_element(yVals.begin(), yVals.end());


	int start = 1000;

	yVals[start] = 1024-a*((yVals[0]/yMax))+500;
	xVals[start] = a*((xVals[0]/xMax));

for (int i = start+1; i < 6000; i++)
{
	yVals[i] = 1024-a*((yVals[i]/yMax))+500;
	xVals[i] = a*((xVals[i]/xMax));

	if(yVals[i] < 1024){
        cout << xVals[i] << " " <<yVals[i] << endl;
				cout << xVals[i+1] << " " <<yVals[i+1] << endl << endl;
        gui->drawLine(xVals[i-1],yVals[i-1],xVals[i],yVals[i],black);
	}
}

//cout << c->getWidth() << endl << c->getHeight();

//gui->drawLine(borders[1][1],borders[1][2],borders[2][1],borders[2][2],black);


  //char s[3] = to_string(borders[0][0]);


    const char buttonName[] = "button";
    MultiText* guiText = c->getGuiText();
    //guiText->add(300,100,s,1);
    //ButtonWidget b(gui, guiText, "hello", 0, 0, 100, 50);
	}
};

int main(int argc, char *argv[]) {
	return GLWin::init(new TestMultiShape());
}
