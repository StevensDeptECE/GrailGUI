#include "util/Callbacks.hh"

#include "util/Ex.hh"

using namespace std;

CallbackHandler::CallbackHandler() {
  for (int i = 0; i < 3; i++) numActions[i] = 1;
}

HashMap<uint32_t> CallbackHandler::actionNameMap(64, 4096);
std::array<uint32_t, 32768> inputMap();

uint32_t CallbackHandler::internalRegisterAction(const char name[], Security s,
                                                 function<void()> action) {
  uint32_t securityIndex = uint32_t(s);
  // SAFE = 0..999, RESTRICTED=1000.1999, ASK=2000..2999
  uint32_t actNum = 1000 * securityIndex + numActions[securityIndex]++;
  // TODO: do something if 1000 action functions exceeded. For now, completely
  // unnecessary
  if (numActions[securityIndex] > 1000) {
    cerr << "Error! action Table is full for security " << securityIndex
         << '\n';
  }
  // cout << "Setting action " << actNum << " for action " << name << '\n';
  setAction(actNum, action);
  actionNameMap.add(name, actNum);
  return actNum;
}

uint32_t CallbackHandler::lookupAction(const char actionName[]) {
  uint32_t *act_code = actionNameMap.get(actionName);
  if (act_code) return *act_code;
  cerr << "Input binding failed: " << actionName << '\n';
  return 0;
}

void CallbackHandler::bind(uint32_t input, const char actionName[]) {
  setEvent(input, lookupAction(actionName));
}

uint32_t CallbackHandler::registerCallback(uint32_t input, const char name[],
                                           Security s,
                                           function<void(void)> action) {
  uint32_t securityIndex = uint32_t(s);
  // SAFE = 0..999, RESTRICTED=1000.1999, ASK=2000..2999
  uint32_t actNum = 1000 * securityIndex + numActions[securityIndex]++;
  // TODO: do something if 1000 action functions exceeded. For now, completely
  // unnecessary
  if (numActions[securityIndex] > 1000) {
    cerr << "Error! action Table is full for security " << securityIndex
         << '\n';
  }
  // cout << "Setting action " << actNum << " for action " << name << '\n';
  setAction(actNum, action);
  actionNameMap.add(name, actNum);
  setEvent(input, lookupAction(name));
  return actNum;
}

void CallbackHandler::doit(uint32_t input) {
  uint32_t act = CallbackHandler::inputMap[input];
  if (act == 0) return;
  auto a = CallbackHandler::actionMap[act];
  a();  // execute the action
}

void CallbackHandler::bind2DOrtho() {
  bind(Inputs::LARROW, "panLeft2D");
  bind(Inputs::RARROW, "panRight2D");
  bind(Inputs::UPARROW, "panUp2D");
  bind(Inputs::DOWNARROW, "panDown2D");
  bind(Inputs::PAGEUP, "zoomIn2D");
  bind(Inputs::PAGEDOWN, "zoomOut2D");
}

void CallbackHandler::bind3D() {
  bind(Inputs::INSERT, "speedTime");
  bind(Inputs::DEL, "slowTime");
  bind(Inputs::RARROW, "panRight3D");
  bind(Inputs::LARROW, "panLeft3D");
  bind(Inputs::PAGEUP, "zoomIn3D");
  bind(Inputs::PAGEDOWN, "zoomOut3D");
  //  bind(Inputs::MOUSE0|Inputs::PRESS|Inputs::ALT, "xyz");
}