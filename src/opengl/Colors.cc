#include "opengl/Colors.hh"
#include <array>

using namespace std;
namespace grail {
	constexpr glm::vec4 u32_to_vec4(uint32_t v) {
		return glm::vec4((v >> 16) / 255.0f, ((v >> 16) & 0xFF) / 255.0f,(v & 0xFF) / 255.0f, 1);
	}
	glm::vec4 black(0, 0, 0, 1);
	glm::vec4 white(1, 1, 1, 1);

	glm::vec4 gray(0.5, 0.5, 0.5, 1);
	glm::vec4 lightgray(0.8, 0.8, 0.8, 1);
	glm::vec4 darkgray(0.3, 0.3, 0.3, 1);

	glm::vec4 red(1, 0, 0, 1);
	glm::vec4 redwhite(1, .9, .9, 1);
	glm::vec4 lightred(1, 0, 0, 1);
	glm::vec4 darkred(0.7, 0, 0, 1);

	glm::vec4 green(0, 1, 0, 1);
	glm::vec4 greenwhite(.9, 1, .9, 1);
	glm::vec4 lightgreen(0.3, 1, 0.3, 1);
	glm::vec4 darkgreen(0, 0.7, 0, 1);

	glm::vec4 blue(0, 0, 1, 1);
	glm::vec4 bluewhite(.9, .9, 1, 1);
	glm::vec4 lightblue(0.3, 0.3, 1, 1);
	glm::vec4 darkblue(0, 0, 0.7, 1);
	
	glm::vec4 orange(1, 0.65, 0, 1);
	glm::vec4 yellow(1, 1, 0, 1);

	glm::vec4 cyan(0, 1, 1, 1);
	glm::vec4 pink(1, 0, 1, 1);
	glm::vec4 purple(1, 0, 1, 1);
	glm::vec4 brown(0.65, 0.16, 0.16, 1);
	glm::vec4 indigo(u32_to_vec4(0x4b0082));
	glm::vec4 violet(u32_to_vec4(0x8f00ff));

	array<glm::vec4,7> red_blue7 {
		red,
		lightred,
		redwhite,
		white,
		bluewhite,
		lightblue,
		blue
	};
	array <glm::vec4,6> continent_colors {
		yellow, blue, cyan, green, pink, red
	};
	array<glm::vec4,7> spectrum {
		red,
		orange,
		yellow,
		green,
		blue,
		indigo,
		violet
	};

	array<glm::vec4,3> primaryColorList = {
		grail::red, grail::green, grail::blue,
	};

	array<glm::vec4,6> primaryColorList2 = {
		grail::red, grail::green, grail::blue,
		grail::darkred, grail::darkgreen, grail::darkblue,
	};

	array<glm::vec4,6> grays = {
		grail::black,
		glm::vec4(0.2,0.2,0.2,1),
		glm::vec4(0.4,0.4,0.4,1),
		glm::vec4(0.6,0.6,0.6,1),
		glm::vec4(0.8,0.8,0.8,1),
		grail::white
	};

};  // namespace grail
