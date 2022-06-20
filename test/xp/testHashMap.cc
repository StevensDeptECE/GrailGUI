#include <iostream>
#include <fstream>
#include <random>
#include "util/BLHashMap.hh"
#include "util/FileUtil.hh"
#include "util/Benchmark.hh"
#include <cstring>

using namespace std;
using namespace grail::utils;

default_random_engine generator;

string generateRandomString(const string &prefix, int n) {
  string randString = prefix;
  uniform_int_distribution<int> distribution(0,25);
  for (int i = 0; i < n; i++) {
    char c = 'a' + distribution(generator);
    randString += c;
  }
  return randString;
}

// function to test loading in the ASCII dictonary to BLHashMap
void loadAsciiDictionary(const char filename[]) {
  uint32_t count = 0;
  uint32_t length;
  char* f;

  // read in the dictionary
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
  delete[] f;
  return;
}

// given a BLHashMap (containing the dictionary), test cases
template<typename T>
void testAsciiDictionary(BLHashMap<T> &dict, const char filename[]) {

  // check dictionary word for word
  uint32_t val;
  uint32_t length;
  char* f;
  FileUtil::readComplete(&f, &length, filename);
  char* word = strtok(f, "\n");
  while (word != nullptr)
  {
    if (!dict.get(word, &val))
      cout << word << " not found.\n";
    word = strtok(nullptr, "\n");
  }

  // pull out the same word _ times
  for (int i = 0; i < 100; i++) {
    if (!dict.get("zymotechnics", &val))
      cout << "zymotechnics not found.\n";
  }
  
  delete [] f;

  // check for bad words (should not exist)
  uint32_t count = 0;
  FileUtil::readComplete(&f, &length, "xp/badwords.txt");
  word = strtok(f, "\n");
  while (word != nullptr)
  {
    if (dict.get(word, &val)) {
      cout << word << " has value "<< val << '\n';
      count++;
    }
    word = strtok(nullptr, "\n");
  }
  cout << count << " bad words found. (BAD)\n";

  // compare similar words (might hash to the same)
  string s;
  count = 0;
  for (int i = 0; i < 100000; i++) {
    s = generateRandomString("#", 8);
    if (dict.get(s.c_str(), &val)) {// SHOULD NEVER HAPPEN
      cout << s << "has value "<< val << '\n';
      count++;
    }
  }
  cout << count << " random words found. (BAD)\n";

  #if 0
  // test case to find word and return value
  char test[] = "zymotechnics";
  if (dict.get(test, &val)) {
    cout << test << "has value "<< val << '\n';
  } else {
    cout << test << " not found\n";
  }
  #endif

  delete[] f;
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
  delete[] f;
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

  delete[] f;

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
  // to warm up
  // same as loadAsciiDictionary(const char filename[])
  // but we don't call it because we need to create dict and keep it
  uint32_t count = 0;
  uint32_t length;
  char* f;

  // read in the dictionary
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

  CBenchmark<>::benchmark(
      "Loading ASCII Dictionary (Makes BLHashMap)", 1e2, [&]() { loadAsciiDictionary(filename); });
#if 1
  CBenchmark<>::benchmark(
      "Loading unordered_map", 10, [&]() { testLoadUnorderedMap(filename); });

  CBenchmark<>::benchmark(
      "Loading unordered_map + ifstream", 10, [&]() { testLoadUnorderedMapIfstream(filename); });

  CBenchmark<>::benchmark(
      "Convert ASCII Dictionary to BLHashMap", 1e2, [&]() { convertAsciiDictionaryToBlockLoader(filename); });

// TODO: more benchmarks for bigger dictionaries
#endif
  CBenchmark<>::benchmark(
      "Loading BLHashMap from disk", 1e3, [&]() { fastLoad(); });

  testAsciiDictionary(dict, filename);

  delete[] f;
}
int main() {
  benchmarkEverything("xp/dict.txt");
//  benchmarkEverything("xp/biggerdict.txt");
  return 0;
}