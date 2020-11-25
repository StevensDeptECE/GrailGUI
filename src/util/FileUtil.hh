#pragma once

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <regex>
#include <string>

#include "opengl/Errcode.hh"
#include "util/Ex.hh"

class FileUtil {
 public:
  static char* readComplete(const char filename[]) {
    int fh = open(filename, O_RDONLY);
    if (fh < 0) return nullptr;
    struct stat info;
    fstat(fh, &info);
    const uint32_t size = info.st_size;
    char* buf = new char[size];
    uint32_t bytesRead = read(fh, buf, size);
    if (bytesRead < size) {
      delete[] buf;
      return nullptr;
    }
    return buf;
  }

  static void readComplete(char** p, uint32_t* len, const char filename[]) {
    int fh = open(filename, O_RDONLY);
    if (fh < 0) return;
    struct stat info;
    fstat(fh, &info);
    const uint32_t size = info.st_size;
    char* buf = new char[size];
    uint32_t bytesRead = read(fh, buf, size);
    if (bytesRead < size) {
      delete[] buf;
      return;
    }
    *p = buf;
    *len = size;
  }

  static bool isDir(const std::string& path) {
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0) return false;
    return S_ISDIR(statbuf.st_mode);
  }

  template <typename T>
  static uint32_t findAll(std::regex* reg, const std::string& dirName,
                          T* handler,
                          void (T::*f)(std::string filePath,
                                       std::string filename)) {
    DIR* dir = opendir(dirName.c_str());
    if (dir == nullptr) throw Ex2(Errcode::DIR_NOT_FOUND, dirName);
    struct dirent* e;

    uint32_t countCalls = 0;
    std::cmatch s;
    while ((e = readdir(dir)) != nullptr) {
      if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;

      if (isDir(dirName + e->d_name)) {
        findAll(reg, (dirName + e->d_name + "/").c_str(), handler, f);
        countCalls++;
        // TODO: Remove, countCalls does not properly count depth of found
        // folders
      } else if (std::regex_search(e->d_name, s, *reg)) {
        (handler->*f)(dirName + e->d_name, e->d_name);
      }
    }
    closedir(dir);
    return countCalls;
  }
  //! This is static because it makes no sense to load fonts over and over again
  //! into multiple browser windows when they should be sharing
  static void buildMapNameToFile(const std::regex* pattern, std::string dirName,
                                 void (*f)(std::string name,
                                           const std::string& path)) {
    DIR* dir = opendir(dirName.c_str());
    if (dir == nullptr) throw Ex2(Errcode::DIR_NOT_FOUND, dirName);
    struct dirent* e;

    std::cmatch s;
    while ((e = readdir(dir)) != nullptr) {
      if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;

      if (isDir(dirName + e->d_name)) {
        buildMapNameToFile(pattern, (dirName + e->d_name + "/").c_str(), f);
      } else if (std::regex_search(e->d_name, s, *pattern)) {
        f(dirName + e->d_name, e->d_name);
      }
    }
    closedir(dir);
  }
  // TODO: Finish creating the method declaration and then implement.

#if 0
	template<typename T>
	static bool find(const std::string& dir, void (*f)(const std::string& filename)) {
		struct dirent* e;
		
		while ((e = readdir(dir)) != nullptr) {
			cout << e->d_name << '\n';
		}
	}
#endif

  void searchDir(const char dirName[]) {
    DIR* dir = opendir(dirName);
    if (dir == nullptr) throw Ex2(Errcode::DIR_NOT_FOUND, dirName);
    struct dirent* e;

    while ((e = readdir(dir)) != nullptr) {
      std::cout << e->d_name << '\n';
    }
  }
};
