#include <iostream>

#include "util/HashMap.hh"
// make a copy of util/Hashmap.hh into src/xp

void convertAsciiDictionaryToBlockLoader() {
  HashMap<uint32_t> dict;
  // while (!f.getline()) {
  dict.add(word, count++);
}

uint32_t val;
if (dict.get("apple", &val)) {
  cout << val

} else {
  cout << "Apple not found";
}
// first create a hashmap and load dictionary in with a unique integer for each
// word
// for (each word) dict.add(word, count++);
// now, modify hash map to save the dictionary into a single block loader

// class HashMapBase : BlockLoader { ???
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

int main() {
  convertAsciiDictionaryToBlockLoader();

  // make a fast dictionary loader that loads from the format you saved
  HashMap<uint32_t> dict("fastloaddict.bld");
}