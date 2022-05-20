#include "data/GapMinderLoader.hh"

#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "data/BlockLoader2.hh"

using namespace std;

inline const char* align(const char* p) {
  return (const char*)(((uint64_t)p + 7) & ~uint64_t(7));
}

GapMinderLoader::GapMinderLoader(const char binaryFile[])
    : BlockLoader(binaryFile) {
  header = (Header*)((char*)mem + getHeaderSize());
  // TODO: add RegionContainer and NamedEntities
  countryCodes = (const char*)align((char*)header + sizeof(Header));
  continents = (const uint8_t*)align(countryCodes + header->numCountries * 3);
  indices =
      (const Dataset*)align((const char*)continents + header->numCountries);
  // for (int i = 0; i < header->numDatasets; i++){
  // cout << "name: " << indices[i].name << endl;
  //}
  data = (const float*)align((char*)indices +
                             sizeof(Dataset) * header->numDatasets);
}

const GapMinderLoader::Dataset* GapMinderLoader::getDataset(
    const char dataset[]) const {
  for (int i = 0; i < header->numDatasets; i++) {
    if (strcmp(indices[i].name, dataset) == 0) {
      return (const Dataset*)&indices[i];
    }
  }
  return nullptr;
}

// originally NaN used cmath's nan(char[]) function, but this proved not to be
// constexpr under Clang. As such, I replaced it with a quiet NaN from numeric
// limits. This should be functionally equivalent if we want to just skip NaN.
// If you're looking to crash on NaN, replace quiet NaN with signaling NaN from
// numeric limits.
//     - ahuston-0
constexpr float NaN = numeric_limits<float>::quiet_NaN();
float GapMinderLoader::getData(uint32_t countryIndex, uint32_t year,
                               const Dataset* d) const {
  if (header->numCountries < countryIndex) {
    return NaN;
  }

  const Dataset::Country& c = d->countries[countryIndex];
  if (year < c.startYear) {
    return NaN;
  }

  uint32_t i = c.startIndex + year - c.startYear;
  if (i >= d->countries[countryIndex + 1].startIndex) {
    return NaN;
  }
  return data[i];
}

#if 0
class Result {
  private:
    const float* data;
    uint32_t startPos;
    //TODO: ignore stride for now, only sequetnial data? uint32_t stride;               //           x y r g b x y r g b x y r g b ...
    uint32_t size;
    //example: Afghanistan GDP          24.1 25.2  26.3  14.2  14.9 ...  Result(start, 1, 42)
    //    abo: n n n n n n n n n n  afg: n n n n n n n n n 
    // example: all data from one year  Result(start, 249 )
  public:
    Result(uint32_t start, uint32_t stride, uint32_t size) : startPos(start), stride(stride), size(size) {}
    float operator [](uint32_t i) const { return data[startPos + i]; }

};
#endif
vector<float> GapMinderLoader::getAllDataOneYear(uint32_t year,
                                                 const Dataset* d) const {
  vector<float> vec(header->numCountries);
  if (year < d->startYear || year > d->endYear) {
    vec = {NaN};
    return vec;
  }

  for (int i = 0; i < header->numCountries; i++) {
    uint32_t index =
        d->countries[i].startIndex + year - d->countries[i].startYear;
    if (i >= d->countries[i + 1].startIndex) {
      vec[i] = (0xffffffff);
    } else if (year < d->countries[i].startYear) {
      vec[i] = (0xffffffff);
    } else {
      vec[i] = (data[index]);
    }
  }

  return vec;
}

vector<float> GapMinderLoader::getAllDataOneCountry(uint32_t countryIndex,
                                                    const Dataset* d) {
  if (header->numCountries < countryIndex) {
    vector<float> vecBad = {NaN};
    return vecBad;
  }

  const int numYears = d->countries[countryIndex + 1].startYear -
                       d->countries[countryIndex].startYear;
  vector<float> vec(numYears);

  for (int i = d->countries[countryIndex].startYear, j = 0;
       i < d->countries[countryIndex + 1].startYear; i++, j++) {
    vec[j] = (data[i]);
  }

  return vec;
}
