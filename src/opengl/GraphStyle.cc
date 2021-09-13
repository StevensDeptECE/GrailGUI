#include "opengl/GraphStyle.hh"


constexpr char faceName[] = "TIMES";
constexpr float titleSize = 24;
constexpr float axisSize = 12;

//		whiskerColors = {grail::cyan, grail::purple};
//		outlineColors = {grail::darkblue, grail::darkgreen};

void GraphStyle::setDefaults() {
	drawBorder = true;
	drawAxes = true;
	drawTicks = true;
	drawLines = true;
	drawMarkers = true;
	//setTickSize(8);
	xTickSize = yTickSize = 8;
	setPointSize(4);
  setBoxWidth(45);
	barColors = {grail::blue, grail::green, grail::red,
		grail::darkblue, grail::darkgreen, grail::darkred };
	barOutlineColors = {grail::gray, grail::black, grail::yellow};
}

void GraphStyle::sparkline() {
	drawBorder = false;
	drawAxes = false;
	drawTicks = false;
	drawLines = true;
	drawMarkers = false;
	setPointSize(4);
  setBoxWidth(45);
}

void GraphStyle::setDefaultColorLists() {

}

GraphStyle* GraphStyle::steelblue;
GraphStyle* GraphStyle::forest;
GraphStyle* GraphStyle::shadow;
GraphStyle* GraphStyle::colorful;

void GraphStyle::classInit() {
	                          
	steelblue = new GraphStyle(faceName, titleSize, axisSize,
														 0x000000, 0x003080, 0x000060, 0x201060, 0xE6F0FF, 0x002070);
	forest    = new GraphStyle(faceName, titleSize, axisSize,
							 							 0x000000, 0x008030, 0x000000, 0x206010, 0xD0FFF0, 0x007030);
	shadow    = new GraphStyle(faceName, titleSize, axisSize,
							 							 0x000000, 0x008030, 0x404040, 0x606060, 0xD0D0D0, 0x203020);
	colorful  = new GraphStyle(faceName, titleSize, axisSize,
							 							 0xC0E000, 0x00C060, 0x2020FF, 0x30E0C0, 0xD020E0, 0x203020);
}
void GraphStyle::classCleanup() {
	delete steelblue;
	delete forest;
	delete shadow;
	delete colorful;
}

#if 0
GraphStyle GraphStyle::steelblue(faceName, titleSize, axisSize)
	.setBorderColor(0x000000)
	.setLineColor(0x003080)
	.setTextColor(0x000000)
	.setBackgroundColor(0xE6F2FF)
	.setPointColor(0x002070)
	.setStyles();

GraphStyle GraphStyle::forest   (faceName, titleSize, axisSize)
	.setBorderColor(0x000000)
	.setLineColor(0x008030)
	.setTextColor(0x000000)
	.setBackgroundColor(0xE6FFF9)
	.setPointColor(0x007030)
	.setStyles();

GraphStyle GraphStyle::shadow   (faceName, titleSize, axisSize)
	.setBorderColor(0x000000)
	.setLineColor(0x006060)
	.setTextColor(0x404040)
	.setBackgroundColor(0xE0E0E0)
	.setPointColor(0x808080)
	.setStyles();

GraphStyle GraphStyle::colorful (faceName, titleSize, axisSize)
	.setBorderColor(0xC0C0C0)
	.setLineColor(0xFF0000)
	.setTextColor(0x30C050)
	.setBackgroundColor(0x502060)
	.setPointColor(0x00FFFF)
	.setStyles();
#endif
