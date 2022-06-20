
#include "opengl/GrailGUI.hh"
class TestTextSimple : public Member {
 public:
  /**
   * @brief
   *
   * @param m The multitext to add to.
   * @param f The font family (Sans, Times, etc.)
   * @param x The x position
   * @param y The y position
   * @param str The text to be displayed
   * @param size The size of the text to be displayed
   */
  void addBasicText(MultiText* m, const char f[], float x, float y, auto str,
                    uint32_t size) {
    // add text object m with font f at x, y
    // Font is Automatically loaded with boldness 10
    const Font* ft = FontFace::get(f, size, 10);
    // testText3 uses the Multitext add function with the length parameter, this
    // one does not, but displays the same.
    m->add(x, y, ft, str);
  }
  TestTextSimple(Tab* tab) : Member(tab, 0, 0.1) {
    Canvas* c = tab->getMainCanvas();
    uint32_t width = c->getWidth(), height = c->getHeight();

    // To create a style, a font is needed, but is overwritten later by the
    // Multitext add.
    const Font* f = FontFace::get("TIMES", 16, FontFace::BOLD);
    Style* s = new Style(f, 0, 0, 0, 0, .2, 0.9, GLWin::TEXT_SHADER);
    std::cout << width << "," << height << '\n';
    // Anything added to Multitext M will adopt the properties of style s.
    MultiText* m = c->addLayer(new MultiText(c, s, 6));
    auto test1 = "Sans";
    auto test2 = "Mono";
    auto test3 = "Times";
    // Defining a new font and style for a new Multitext layer
    const Font* foo = FontFace::get("TIMES", 16, FontFace::BOLD);
    Style* sty = new Style(foo, 0, 0, 0.9, 0.9, 0, 0.9, GLWin::TEXT_SHADER);
    MultiText* new_m = c->addLayer(new MultiText(c, sty, 7));
    addBasicText(m, "SANS", 20, 200, test1, 40);
    addBasicText(m, "MONO", 40, 100, test2, 30);
    addBasicText(new_m, "TIMES", 60, 300, test3, 10);
  }
};
void grailmain(int argc, char* argv[], GLWin* win, Tab* defaultTab) {
  win->setTitle("test text simple");
  new TestTextSimple(defaultTab);
}