#include <unistd.h>

#include "opengl/GrailGUI.hh"
class SimpleOutlineTest : public Member {
  /**
   * @brief
   *
   * @param outline Stores the map of glyphs
   * @param gui Stores the StyledMultiShape we draw to
   * @param current Stores the current character
   * */
 private:
  std::unordered_map<uint8_t, std::vector<float>> outline;
  StyledMultiShape2D* gui;
  uint8_t current;

 public:
  SimpleOutlineTest(Tab* tab) : Member(tab, 0, 0) {
    MainCanvas* c = tab->getMainCanvas();
    glm::mat4* proj = c->getProjection();  // Get Porjection Matrix
    *proj = glm::scale(
        *proj, glm::vec3(0.25, -0.25, 1));  // Scale and translate the glyphs to
                                            // make them upright and centered
    *proj = glm::translate(
        *proj,
        glm::vec3(300, -2000, 0));  // TODO: change values in translate to make
                                    // every glyph completely visible
    gui = c->getGui();
    const Style* s = tab->getDefaultStyle();  // Get the default style
    outline = s->f->getOutlinePoints();  // Get the map of points stored in the
                                         // default font
    current = 'a';  // Characters are added in order, so they're pushed in in
                    // order, so we start from a
  }

  void update() {
    //  Find out in point list where outer/inner are separated
    gui->clear();  // clear the gui so we can start drawing the character
    gui->drawPolyline(outline.at(current).data(),
                      outline.at(current).size() / 2, grail::black);
    gui->update();         // tell the gui to update
    current++;             // Move to next character
    if (current > 1000) {  // if over 1000 characters have been drawn, go back
                           // to the beginning, see line 75 of GLWinFonts.cc
      current = 'a';
    }
    sleep(1);  // TODO: Rendering too fast caused issues on windows, some
               // characters would be skipped, or things would not be drawn,
               // sleep is to keep the rendering slow enough to not cause issues
  }

  // void render() {}
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test Simple Outline");
  w->setSize(1920, 512);
  new SimpleOutlineTest(defaultTab);
}