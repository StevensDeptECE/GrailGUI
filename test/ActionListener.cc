#include "ActionListener.hh"
#include <thread>
#include <unordered_map>

class Inputs;
std::thread ActionListener::t;
Inputs *ActionListener::i;
std::unordered_map<int, ActionListener::Mouse*> ActionListener::mice;
int ActionListener::die;
