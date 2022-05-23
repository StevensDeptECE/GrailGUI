#include "util/Config.hh"
#include "util/Prefs.hh"

int main() {
  Config conf("grail.conf");
  conf.mandatory("host", Config::Type::STRING, "localhost", "port",
                 Config::Type::U16, 80);

  Prefs prefs(conf);

  uint16_t port = conf.defaulted("port", 80);
}
