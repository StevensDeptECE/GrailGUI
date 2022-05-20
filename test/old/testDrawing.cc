#include "opengl/GrailGUI.hh"
#include <cmath>

//#include "grail/BarGraph.hh"
//#include "grail/Grail.hh"
//#include "grail/LineGraph.hh"
using namespace std;

class TestDrawing : public Grail {
 public:
  void init() {
    Style* s = getDefaultStyle();
    const Font* font = getDefaultFont();
    Canvas* c = currentTab()->getMainCanvas();

    StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(s));
    MultiText* t = c->addLayer(new MultiText(s, 20));
    // t->init();

    vector<double> testx;
    vector<vector<double>> testy;

    int numGraphs = 6;
    for (int i = 0; i < numGraphs; i++) testy.push_back(vector<double>());

    std::string title("Sine wave, Exponential, and Logarithm");

    for (double i = 0; i < 7.5; i += 0.25) {
      testx.push_back(i);
      testy[0].push_back(2 * sin(i) + 5);
      testy[1].push_back(4 * sin(i) + 5);
      testy[2].push_back(6 * sin(i) + 5);
      testy[3].push_back(8 * sin(i) + 5);
      testy[4].push_back(10 * sin(i) + 5);
      testy[5].push_back(12 * sin(i) + 5);
      /*
      testy[1].push_back(pow(1.5, i));
      testy[2].push_back(6*log(i+1));
      */
    }

    vector<char> shapes(numGraphs);
    shapes[0] = 't';
    shapes[1] = 'r';
    shapes[2] = 'c';
    shapes[3] = 't';
    shapes[4] = 'r';
    shapes[5] = 'c';

#if 0
		for (auto i = testx.begin(); i != testx.end(); ++i)
    		std::cout << *i << ' ';
		std::cout << '\n';

		for (int j = 0; j < testy.size(); j++) {
			for (auto i = testy[j].begin(); i != testy[j].end(); ++i)
				std::cout << *i << ' ';
			std::cout << '\n';
		}
#endif
    testy.push_back(vector<double>());

    for (double i = 0; i < 7.5; i += 0.5) {
      testx.push_back(i);
      testy[0].push_back(((0.5 * i - 3)) * (0.5 * i - 3) + 3);
    }

#if 0
		for (auto i = testx.begin(); i != testx.end(); ++i)
    		std::cout << *i << ' ';
		std::cout << '\n';

		for (int j = 0; j < testy.size(); j++) {
			for (auto i = testy[j].begin(); i != testy[j].end(); ++i)
				std::cout << *i << ' ';
			std::cout << '\n';
		}
#endif
    BarGraph* b = new BarGraph(title, t, m, s, 50, 50, 600, 400, testx, testy);
    LineGraph* g =
        new LineGraph(title, t, m, s, 50, 50, 900, 600, shapes, testx, testy);
  }
};

int main(int argc, char* argv[]) {
  return Grail::init(new TestDrawing(), 1024, 1024, "out.cml");
}
