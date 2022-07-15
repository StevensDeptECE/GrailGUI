#pragma once

#include <array>
#include <string>

#include "Ex.hh"

class Logger {
 public:
  enum class Level { LOGINFO, LOGDEBUG, LOGWARN, LOGERROR, LOGCRITICAL };

 private:
  std::ostream* fh;
  const std::array<std::string, 5> levels = {"INFO", "DEBUG", "WARN", "ERROR",
                                             "CRITICAL"};

 public:
  Logger(std::ostream* fh) : fh(fh) {}
  void log(Level level, const std::string_view& str) {
    (*fh) << levels[(int)level] << " " << str << '\n';
  }
  void info(const std::string_view& str) { log(Level::LOGINFO, str); }
  void debug(const std::string_view& str) { log(Level::LOGDEBUG, str); }
  void warn(const std::string_view& str) { log(Level::LOGWARN, str); }
  void error(const std::string_view& str) { log(Level::LOGERROR, str); }
  void critical(const std::string_view& str) { log(Level::LOGCRITICAL, str); }
  void flush() { fh->flush(); }
  ~Logger() { fh->flush(); }
};

Logger logger(&std::cout);
