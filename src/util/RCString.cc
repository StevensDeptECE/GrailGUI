#include "util/RCString.hh"

char* RCString::pool =
    (char*)new std::aligned_storage<4UL * 1024 * 1024 * 1024>;
uint32_t RCString::current = 0;
