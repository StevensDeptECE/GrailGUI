#include "opengl/Canvas.hh"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Style.hh"
//TODO: gl.h can't be included multiple times, really ugly.
// is there a fix?
//#include <GL/gl.h>
#include "opengl/util/Camera.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"

using namespace std;

Canvas::~Canvas() {
  cleanup();
}

void Canvas::cleanup() {
  for (auto s : layers) {
    delete s;
  }
  layers.clear();
  delete cam;
  cam = nullptr;
} 

void Canvas::render() {
  // std::cout << projection << std::endl;
  Shader::useShader(style->getShaderIndex())->setMat4("projection", projection);
  glViewport(vpX,w->height-vpH-vpY,vpW,vpH); 

  for(const auto& l : layers){
    l->render();
  }
}

Camera* Canvas::setLookAtProjection(float eyeX, float eyeY, float eyeZ,
                                 float lookAtX, float lookAtY, float lookAtZ,
                                float upX, float upY, float upZ) {
  cam = new Camera(w->width, w->height, glm::vec3(upX, upY, upZ));
  cam->setCamPos(glm::vec3(eyeX, eyeY, eyeZ));
  cam->setLookingAt(glm::vec3(lookAtX, lookAtY, lookAtZ));
  return cam;
}

MainCanvas::MainCanvas(GLWin* parent) :
	Canvas(parent, parent->getDefaultStyle(),
				 0, 0, parent->getWidth(), parent->getHeight(),
				 parent->getWidth(), parent->getHeight()) {
	gui = new StyledMultiShape2D(this, parent->getGuiStyle());
	guiText = new MultiText(this, parent->getGuiTextStyle(), 16384);
	menu = new StyledMultiShape2D(this, parent->getMenuStyle());
	menuText = new MultiText(this, parent->getMenuTextStyle(), 16384);
}

MainCanvas::~MainCanvas() {
	delete gui;
	delete guiText;
	delete menu;
	delete menuText;
}
void MainCanvas::addButton(const char text[],
													 float x, float y, float w, float h) {
	gui->drawRectangle(x, y, w, h, style->fg);
	guiText->add(x, y, style->f, text, strlen(text));
}

void MainCanvas::addLabel(const char text[],
													float x, float y, float w, float h) {
	guiText->add(x, y, style->f, text, strlen(text));
}

void MainCanvas::addMenu(const string menu[], uint32_t numStrings,
                         float x, float y) {
	const Font* f = style->f;
	for (int i = 0; i < numStrings; i++, y += f->getHeight()) {
		menuText->add(x, y, f, menu[i].c_str(), menu[i].length());
	}
}

void MainCanvas::init() {
	Canvas::init(); // call parent for normal initialization
  // initialize the GUI layers (does not have to be at the end, but in render it does)
  gui->init();
  guiText->init();
  menu->init();
  menuText->init();
}

void MainCanvas::render() {
  Canvas::render(); // call parent's render
  // then render the GUI layer on top of everything
  gui->render();
  guiText->render();
  menu->render();
  menuText->render();
}

void MainCanvas::cleanup() {
  Canvas::cleanup();
	//TODO: add cleanup for Shape? Does it deallocate memory from graphics card?
//  gui->cleanup();
//  guiText->cleanup();
//  menu->cleanup();
//  menuText->cleanup();
}
