#pragma once

#include <unistd.h>

#include <iostream>
#include <thread>
#include <unordered_map>

#include "../src/input/linux/Inputs.hh"
class ActionListener {
 public:
  /* Used with registerCallback() */
  enum Events { MOUSE };

  /* MouseEvent identifiers */
  enum class MouseData { LEFT, RIGHT, MIDDLE };

  enum class MouseType { PRESSED, RELEASED, ENTERED, EXITED, CLICKED };

  struct MouseEvent {
    MouseType type;
    MouseData data;
    MouseEvent(MouseType t, MouseData d) : type(t), data(d) {}
  };

 private:
  static Inputs *i;
  static std::thread t;
  static int die;

  struct Mouse {
    int left, right, middle;
    int id;
    void (*f)(MouseEvent e);
    Mouse() { std::cout << "Default constructor called\n"; }
    Mouse(void (*f)(MouseEvent e)) : f(f) { left = right = middle = 0; }
  };

  static std::unordered_map<int, Mouse *> mice;

  static void listen(int id) {
    while (!die) {
      int left = i->get_left(id);
      int right = i->get_right(id);
      int middle = i->get_middle(id);
      // 1. Check for state change
      // 2. Look at state
      if (mice[id]->left != left) {
        if (left)
          mice[id]->f(MouseEvent(MouseType::PRESSED, MouseData::LEFT));
        else
          mice[id]->f(MouseEvent(MouseType::RELEASED, MouseData::LEFT));
      }
      if (mice[id]->right != right) {
        if (right)
          mice[id]->f(MouseEvent(MouseType::PRESSED, MouseData::RIGHT));
        else
          mice[id]->f(MouseEvent(MouseType::RELEASED, MouseData::RIGHT));
      }
      if (mice[id]->middle != middle) {
        if (middle)
          mice[id]->f(MouseEvent(MouseType::PRESSED, MouseData::MIDDLE));
        else
          mice[id]->f(MouseEvent(MouseType::RELEASED, MouseData::MIDDLE));
      }

      mice[id]->left = left;
      mice[id]->right = right;
      mice[id]->middle = middle;
      usleep(50);
    }
  }

 public:
  ActionListener(Inputs *i) {
    ActionListener::i = i;
    ActionListener::die = 0;
  }

  ~ActionListener() {
    std::cout << "Killing ActionListener thread..." << std::endl;
    ActionListener::die = 1;
    t.join();
  }

  void registerCallback(Events e, int id, void (*f)(MouseEvent e)) {
    switch (e) {
      case Events::MOUSE:
        mice[id] = new Mouse(f);
        t = std::thread(listen, id);
    }
  }
};
