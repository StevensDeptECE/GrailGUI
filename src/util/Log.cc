#include "Log.hh"

#include <fcntl.h>
#include <unistd.h>
Log::Log() {}

void Log::setLogFile(const char filename[]) {
  fh = creat(filename, 0644);
  if (fh < 0) throw "IOEXception";  // TODO: Put in real exception object!
}

Log::~Log() {
  if (fh >= 0) close(fh);
}
