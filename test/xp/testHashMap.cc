#include <iostream>
#include <fstream>
#include "util/BLHashMap.hh"
#include "util/FileUtil.hh"
#include "util/Benchmark.hh"
#include <cstring>
// make a copy of Hashmap.hh into src/xp

using namespace std;
using namespace grail::utils;

// function to test loading in the ASCII dictonary to BLHashMap
void loadAsciiDictionary(const char filename[]) {
  uint32_t count = 0;
  uint32_t length;
  char* f;

  // read in the dictionary (213k words)
  FileUtil::readComplete(&f, &length, filename);
  char* word = strtok(f, "\n");
  // create hashmap and load dictionary in with a unique integer for each word
  BLHashMap<uint32_t> dict = BLHashMap<uint32_t>(length, length/5, length/5);
  while (word != nullptr)
  {
    dict.add(word, count++);
    //cout << word << ": " << count << '\n';
    word = strtok(nullptr, "\n");
  }
}

// given a BLHashMap (containing the dictionary), test cases
template<typename T>
void testAsciiDictionary(BLHashMap<T> &dict) {
  // TODO: add test cases
  // pull out the same word _ times
  // check for commonly misspelled words
  // compare similar words (might hash to the same)
  // check word for word

  // test case to find word and return value
  uint32_t val;
  char test[] = "zymotechnics";
  if (dict.get(test, &val)) {
    cout << test << "has value "<< val << '\n';
  } else {
    cout << test << " not found\n";
  }
}

// benchmark against C++ unordered_map
void testLoadUnorderedMap(const char filename[]) {
  unordered_map<string, uint32_t> dict;
  uint32_t count = 0;
  uint32_t length;
  char* f;

  // read in the dictionary
  FileUtil::readComplete(&f, &length, filename);
  char* word = strtok(f, "\n");
  while (word != nullptr)
  {
    dict[word] = count++;
    //cout << word << ": " << count << '\n';
    word = strtok(nullptr, "\n");
  }
}

// benchmark against C++ unordered_map (using inefficient ifstream)
void testLoadUnorderedMapIfstream(const char filename[]) {
  unordered_map<string, uint32_t> dict;
  uint32_t count = 0;
  uint32_t length;
  string word;

  // read in the dictionary (using unordered_map + ifstream)
  ifstream f(filename);
  while ( getline(f, word) )
  {
    dict[word] = count++;
  }
}


void convertAsciiDictionaryToBlockLoader(const char filename[]) {
  uint32_t count = 0;
  uint32_t length;
  char* f;

  // read in the dictionary (213k words)
  FileUtil::readComplete(&f, &length, filename);
  char* word = strtok(f, "\n");
  // create hashmap and load dictionary in with a unique integer for each word
  BLHashMap<uint32_t> dict = BLHashMap<uint32_t>(length, length/5, length/5);
  while (word != nullptr)
  {
    dict.add(word, count++);
    //cout << word << ": " << count << '\n';
    word = strtok(nullptr, "\n");
  }
  
  dict.writeFile("fastloaddict.bdl");

  // now, modify hash map to save the dictionary into a single block loader
  // write out the block loader format
  // 4 bytes magic number, 2 bytes  type , ....
  // save here as "fastloaddict.bld"

  // allocate assuming each word = 2 bytes.
  // length of file = header + number of bytes in words
  // conservative estimate of words = (length of file - header) / 2
  // use: malloc() to allocate the memory
  // read in the file into the memory you allocated
  // calculate the 3 pointers: symbols, table, nodes
  // use realloc() to give back the excess (because you had to over-estimate)
  // add and fill in the table
}

void fastLoad() {
  BLHashMap<uint32_t> dict("fastloaddict.bdl");
}

void benchmarkEverything(const char filename[]) {
  loadAsciiDictionary(filename); // to warm up

  CBenchmark<>::benchmark(
      "Loading ASCII Dictionary (Makes BLHashMap)", 1e2, [&]() { loadAsciiDictionary(filename); });

  CBenchmark<>::benchmark(
      "Loading unordered_map", 10, [&]() { testLoadUnorderedMap(filename); });

  CBenchmark<>::benchmark(
      "Loading unordered_map + ifstream", 10, [&]() { testLoadUnorderedMapIfstream(filename); });
  
  #if 0
  CBenchmark<>::benchmark(
      "Loading unordered_map", 1e2, [&]() { testAsciiDictionary(filename); });
  #endif

  CBenchmark<>::benchmark(
      "Convert ASCII Dictionary to BLHashMap", 1e2, [&]() { convertAsciiDictionaryToBlockLoader(filename); });

// TODO: more benchmarks for bigger dictionaries

  CBenchmark<>::benchmark(
      "Loading BLHashMap from disk", 1e2, [&]() { fastLoad(); });

}
int main() {
  benchmarkEverything("xp/dict.txt");
//  benchmarkEverything("xp/biggerdict.txt");

//TODO: Segmentation fault in release mdoe
  return 0;
}