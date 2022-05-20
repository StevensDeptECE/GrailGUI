#pragma once

#include <array>
#include <cstdint>
#include <functional>

#include "util/HashMap.hh"

class CallbackHandler {
 public:
  /*
    map input events, like pressing a key, or clicking alt-mouse button 1
    to an action number. Actions are internally stored in a separate table
    this allows defining key bindings

                TODO: Later, build a more sophisticated modal input where this
    can change in a mode, a second input map would override the first, allowing
    different mappings for different states.
  */
  CallbackHandler();

  template <typename T>
  using CallbackFunc = void (T::*)(void);

  inline static std::array<uint32_t, 32768> inputMap;
  /*
        map an integer code to a function to execute
        the actions are all the publicly available performance the code can DO
  */
  inline static std::array<std::function<void(void)>, 4096> actionMap;
  static HashMap<uint32_t> actionNameMap;
  uint32_t lookupAction(const char actionName[]);
  //	static GLWin* w;
  void setEvent(uint32_t e, uint32_t a) { inputMap[e] = a; }

  void setEvent(uint32_t key, uint32_t mod, uint32_t a) {
    setEvent((mod << 9) | key, a);
  }

  void setAction(uint32_t a, std::function<void()> action) {
    actionMap[a] = action;
  }
  enum class Security {
    SAFE,        // safe for a remote server to trigger this function
    RESTRICTED,  // only the local user can execute this function
    ASK  // a remote user or server may request this but it will trigger a popup
         // asking local user to approve
  };
  std::array<uint32_t, 3> numActions;  // keep track of how many of each kind of
                                       // operations there are
  uint32_t internalRegisterAction(const char name[], Security s,
                                  std::function<void()> action);
  template <typename T, typename U>
  uint32_t internalRegisterAction(const char name[], Security s,
                                  CallbackFunc<T> callback, U* ptr) {
    auto cb_funcptr = std::bind(callback, ptr);
    return internalRegisterAction(name, s, cb_funcptr);
  }
  // registerAction("myFunc", Security::RESTRICTED, myFunc)
  // bind an input event to an action Name. looks up offsets into arrays
  void bind(uint32_t input, const char actionName[]);
  void bind(const char inputCmd[], const char actionName[]);
#define quote(a) #a

  template <typename T, typename U>
  void registerAction(Security security, CallbackFunc<T> func, U* ptr) {
    internalRegisterAction(quote(func), security, func, ptr);
  }
  uint32_t registerCallback(uint32_t input, const char name[], Security s,
                            std::function<void(void)> action);
  template <typename T, typename U>
  uint32_t registerCallback(uint32_t input, const char name[], Security s,
                            CallbackFunc<T> callback, U* ptr) {
    auto cb_funcptr = std::bind(callback, ptr);
    return registerCallback(input, name, s, cb_funcptr);
  }

  template <typename T, typename U>
  void bindEvent(uint32_t inp, CallbackFunc<T> func, U* ptr) {
    registerCallback(inp, quote(func), Security::SAFE, func, ptr);
  }

  void doit(uint32_t input);

  void bind2DOrtho();
  void bind3D();

  enum Inputs {
    INSERT = 260,
    DEL = 261,
    RARROW = 262,
    LARROW = 263,
    UPARROW = 265,
    DOWNARROW = 264,
    PAGEUP = 266,
    PAGEDOWN = 267,
    HOME = 268,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    WHEELUP = 401,
    WHEELDOWN = 399,
    MOUSE1 = 1,
    MOUSE2 = 2,
    MOUSE3 = 3,
    MOUSE4 = 4,
    KEY_RELEASE = 512,
    KEY_REPEAT = 1024,
    MOUSE0_PRESS = 8,
    MOUSE0_RELEASE = 0,
    CTRL = 2048,
    SHIFT = 4096,
    ALT = 8192
  };
};