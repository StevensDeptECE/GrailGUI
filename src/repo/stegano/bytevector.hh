#include <cstring>
#include <iomanip>
#include <iostream>

// TODO: Look into https://en.wikipedia.org/wiki/Six-bit_character_code
class byte_vec : public std::basic_string<uint8_t> {
  friend std::ostream& operator<<(std::ostream& os, const byte_vec& bv) {
    os << std::uppercase << std::hex;
    for (auto c : bv)
      os << "0x" << std::setw(2) << std::setfill('0') << static_cast<int>(c)
         << ' ';
    os << std::nouppercase << std::dec;
    return os;
  }
};
