#include <iostream>
#include "HashMap.hh"
#include "util/FileUtil.hh"
#include "BlockLoader2.hh"
#include <cstring>
// make a copy of Hashmap.hh into src/xp

using namespace std;

void convertAsciiDictionaryToBlockLoader() {
  uint32_t count = 0;
  uint32_t length;
  char* f;

  // read in the dictionary (213k words)
  FileUtil::readComplete(&f, &length, "xp/dict.txt");
  char* word = strtok(f, "\n");
  // create hashmap and load dictionary in with a unique integer for each word
  HashMap<uint32_t> dict = HashMap<uint32_t>(length, length/5, length/5);
  while (word != NULL)
  {
    dict.add(word, count++);
    //cout << word << ": " << count << '\n';
    word = strtok(NULL, "\n");
  }
  
  // test case to find word and return value
  uint32_t val;
  char test[] = "zymotechnics";
  if (dict.get(test, &val)) {
    cout << test << "has value "<< val << '\n';
  } else {
    cout << test << " not found\n";
  }
  
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

int main() {
  convertAsciiDictionaryToBlockLoader();

// make a fast dictionary loader that loads from the format you saved
//  HashMap<uint32_t> dict("fastloaddict.bld");

/* using FileUtil
  char* f = FileUtil::readComplete("xp/dict.txt");
  char* word = strtok(f, "\n");
  while (word != NULL)
  {
    cout << word << '\n';
    word = strtok(NULL, "\n");
  }
*/

return 0;
}