#include "csp/IPV4Socket.hh"
#include "csp/XDLRequest.hh"
//#include "csp/csp.hh"
//#include <cstdlib>
#include <set>

#include "opengl/GrailGUI.hh"
#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "util/HashMap.hh"
#include "xdl/XDLCompiler.hh"
#include "xdl/std.hh"
using namespace std;

/**
 * @brief Renders incoming data based on the metadata from Buffer to screen
 *
 * Renderer has state to track current location on the screen (visual cursor)
 * and iterators within the object to track position within the data.
 * For a large object such as a list, the renderer might only display the
 * visible subset and stop.
 *
 */
class Renderer {
 public:
  /**
   * @brief A struct containing the top-left corner, width, and height of a
   * bounding box
   */
  struct BoundBox {
    float x0, y0;
    uint32_t width, height;

    BoundBox(float x, float y, uint32_t w, uint32_t h)
        : x0(x), y0(y), width(w), height(h) {}
  };
  typedef void (Renderer::*Method)();

 private:
  XDLType* it;           /**< the iterator to the XDLType to be drawn */
  StyledMultiShape2D* m; /**< Stores and renders graphics onto the screen */
  MultiText* t;          /**< Stores and renders text onto the screen */

  /**< A dynamic array of top level structures, used for iterating between
   * top-level structures */
  DynArray<const XDLType*>& requests;

  /**< A hashmap of the top-level structures, used for quick lookups */
  HashMap<const XDLType*>& byName;
  std::unordered_map<DataType, Method> renderMap;

  Buffer& in; /**< A buffer containing the data to be rendered */

  /**< Store the data for this object so we can go backwards and possibly save
   * locally */
  DynArray<uint8_t> dataBuffer;
  BoundBox bounds;

  /**< Size for each row of data, related to font + spacing if it's text */
  float rowSize;
  float x, y; /**< Current render position */

  void registerIterator(XDLType* xdl_it) {
    CompoundType* comp = dynamic_cast<CompoundType*>(xdl_it);
    if (comp != nullptr) {
      it = (XDLIterator*)xdl_it->begin(in);
    } else {
      auto method = renderMap[xdl_it->getDataType()];
      (this->*method)();
    }
  }

  void registerRenderers() {
    registerRenderer(DataType::STRUCT8, &Renderer::renderStructAcross);
    registerRenderer(DataType::LIST16, &Renderer::renderListDown);
    registerRenderer(DataType::U32, &Renderer::renderU32);
    registerRenderer(DataType::U64, &Renderer::renderU64);
    registerRenderer(DataType::F32, &Renderer::renderF32);
    registerRenderer(DataType::F64, &Renderer::renderF64);
  }

 public:
  /**
   * @brief Construct a new Renderer object
   *
   * @param it An XDLIterator to iterate through the data
   * @param c A Canvas to draw on
   * @param requests A dynamic array of top-level structures
   * @param byName A hashmap of top-level structures
   * @param in A buffer containing the data to be rendered
   */
  Renderer(XDLType* it, MainCanvas* c, DynArray<const XDLType*>& requests,
           HashMap<const XDLType*>& byName, Buffer& in)
      : it(it),
        m(c->getGui()),
        t(c->getGuiText()),
        dataBuffer(65536),
        requests(requests),
        byName(byName),
        renderMap(65536),
        in(in),
        bounds(0, 0, c->getWidth(), c->getHeight()),
        rowSize(20),
        x(bounds.x0),
        y(bounds.y0 + rowSize) {
    //    registerIterator(it);
    registerRenderers();
  }

  /**
   * @brief Construct a new Renderer object
   *
   * @param it An XDLIterator to iterate through the data
   * @param c A Canvas to draw on
   * @param requests A dynamic array of top-level structures
   * @param byName A hashmap of top-level structures
   * @param in A buffer containing the data to be rendered
   * @param b A bounding box containing the top-left corner, width, and height
   */
  Renderer(XDLType* it, MainCanvas* c, DynArray<const XDLType*>& requests,
           HashMap<const XDLType*>& byName, Buffer& in, const BoundBox& b)
      : it(it),
        m(c->getGui()),
        t(c->getGuiText()),
        requests(requests),
        byName(byName),
        renderMap(65536),
        in(in),
        dataBuffer(65536),
        bounds(b) {
    registerIterator(it);
    registerRenderers();
  }

  /**
   * @brief Construct a new Renderer object
   *
   * @param it An XDLIterator to iterate through the data
   * @param c A Canvas to draw on
   * @param requests A dynamic array of top-level structures
   * @param byName A hashmap of top-level structures
   * @param in A buffer containing the data to be rendered
   * @param x0 The top-left x coordinate of the bounding box
   * @param y0 The top-left y coordinate of the bounding box
   * @param width The width of the bounding box
   * @param height The height of the bounding box
   */
  Renderer(XDLType* it, MainCanvas* c, DynArray<const XDLType*>& requests,
           HashMap<const XDLType*>& byName, Buffer& in, const float& x0,
           const float& y0, const uint32_t& width, const uint32_t& height)
      : Renderer(it, c, requests, byName, in, BoundBox(x0, y0, width, height)) {
  }

  void advance(XDLIterator* endPage) {
    // it = endPage + 1;
  }

  Method* rendererFind(DataType t) {
    auto i = renderMap.find(t);
    if (i == renderMap.end()) return nullptr;
    return &i->second;
  }

  Method* rendererFind(XDLType* t) {
    XDLIterator* comp = dynamic_cast<XDLIterator*>(t);
    if (comp) {
      auto i = renderMap.find((comp->getUnderlying()->getDataType()));
      if (i != renderMap.end()) return &i->second;
    }
    return nullptr;
  }

  void registerRenderer(DataType t, Method m) { renderMap[t] = m; }
  void renderListDown();
  void renderStructAcross();
  void renderSubStructAcross(Struct* s);
  void renderStructNameValue();
  void renderObjectMetadataAcross();
  void renderU32();
  void renderU64();
  void renderF32();
  void renderF64();
  void update();
};

void Renderer::update() {
  Method* elementRenderer = rendererFind(it);
  if (!elementRenderer) {
    cerr << "bad renderer";
    return;
  }
  (this->**elementRenderer)();
}

void Renderer::renderListDown() {
  GenericList::Iterator* i = dynamic_cast<GenericList::Iterator*>(it);
  XDLType* elementType = i->getListType();
  if (i == nullptr) {
    cerr << "Expected generic list!";
    return;
  }
  Method* elementRenderer = rendererFind(elementType->getDataType());
  if (!elementRenderer) {
    cerr << "bad renderer";
    return;
  }
  for (; !*i && y < bounds.height; ++i, y += rowSize) {
    (this->**elementRenderer)();
  }
  // endPage = i;
}

void Renderer::renderObjectMetadataAcross() {
  Struct::Iterator* i = dynamic_cast<Struct::Iterator*>(it);
  Struct* s = i->getStruct();
  if (i == nullptr) {
    cerr << "Expected generic list!";
    return;
  }
  for (uint32_t i = 0; i < s->getMemberCount(); i++) {
    /*
     * Struct::Member is protected, need to use s->getMembers(i).type to get
     * XDLType
     */
    // const Struct::Member& m = s->getMembers(i);
    // string memberName = s->getMemberName();
    // TODO: print across page
  }
  // endPage = current iterator position;
}

void Renderer::renderStructAcross() {
  Struct::Iterator* startPage = dynamic_cast<Struct::Iterator*>(it);
  if (startPage == nullptr) {
    cerr << "Expected generic list!";
    return;
  }
  Struct* s = startPage->getStruct();
  Method elementRenderer;

  for (Struct::Iterator j = *startPage; !j; ++j) {
    const XDLType* m = *j;
    if (m->getDataType() == DataType::STRUCT8) {
      renderSubStructAcross((Struct*)m);
    } else {
      Method* elementRenderer = rendererFind(m->getDataType());
      if (!elementRenderer) {
        cerr << "bad renderer datatype=" << uint32_t(m->getDataType())
             << " typename=" << m->getTypeName() << '\n';
      }
      (this->**elementRenderer)();
      x += 150;
    }
  }
}

void Renderer::renderSubStructAcross(Struct* s) {
  Method elementRenderer;

  for (Struct::Iterator j(s); !j; ++j) {
    const XDLType* m = *j;
    if (m->getDataType() == DataType::STRUCT8) {
      renderSubStructAcross((Struct*)m);
    } else {
      Method* elementRenderer = rendererFind(m->getDataType());
      if (!elementRenderer) {
        cerr << "bad renderer datatype=" << uint32_t(m->getDataType())
             << " typename=" << m->getTypeName() << '\n';
      }
      (this->**elementRenderer)();
    }
    x += 150;
  }
}

void Renderer::renderStructNameValue() { return; }
void Renderer::renderU32() {
  Buffer& incpy = in;
  t->add(x, y, in.readU32());
}
void Renderer::renderU64() { t->add(x, y, in.readU64()); }
void Renderer::renderF32() { t->add(x, y, in.readF32()); }
void Renderer::renderF64() { t->add(x, y, in.readF64()); }

/*
  This generic client demonstrates the ability to send a request to an XDL
  server, and draw the data returned on screen, paging through the data
  if it's too big.

  Known issues at this time:
  1. Not all data types are implemented yet, ie don't send JPEG or BLOB.
   We focus mostly on numbers, strings, lists and struct for proof of concept.

  2. Formatting is very ad hoc and not efficient. We need to come up with a
  way to define good formatting that is fast.  C++ recently added format, we
  should take a look
*/

class GraphicalXDLClient : public GLWin {
 private:
  IPV4Socket s;
  uint32_t reqID;
  const XDLType* latest;
  DynArray<const XDLType*> requests;
  HashMap<const XDLType*> byName;
  XDLIterator* currentPos;
  Renderer* r;

  set<DataType> structTypes = {DataType::STRUCT8, DataType::STRUCT16,
                               DataType::STRUCT32};

  void add(const XDLType* t) {
    requests.add(t);
    byName.add(t->getTypeName().c_str(), t->getTypeName().length(), t);
  }

 public:
  GraphicalXDLClient(const char* const ip, int port, uint32_t req)
      : GLWin(0x000000, 0xCCCCCC, "Grail Graphical Generic Client"),
        s(ip, port),
        reqID(req),
        requests(64),
        byName(64) {}
  void readData(uint32_t req) {
    s.send(req);
    Buffer& in = s.getIn();
    in.displayRawRead();
    XDLCompiler compiler("");
    // SymbolTable st(&compiler);
    XDLType* latest = (XDLType*)XDLType::readMeta(&compiler, in);
    add(latest);
    float rowSize = 20;

    r = new Renderer(latest->begin(in), currentTab()->getMainCanvas(), requests,
                     byName, in);
    // TODO: if we call readData multiple times, delete the old renderer before
    // replacing it
  }
  void init() {
    readData(reqID);
    update();
  }

  void update() {
    if (dirty) {
      r->update();
    }
  }
};
int main(int argc, char* argv[]) {
  const char* const ip = argc > 1 ? argv[1] : "127.0.0.1";
  int port = argc > 2 ? atoi(argv[2]) : 8060;
  uint32_t req = argc > 3 ? atoi(argv[3]) : 0;
  GLWin::init(new GraphicalXDLClient(ip, port, req));
  return 0;
}
