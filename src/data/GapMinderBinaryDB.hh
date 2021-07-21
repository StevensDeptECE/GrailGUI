#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "data/BlockLoader.hh"


constexpr float NaN = 0xffffffff;

class GapMinderBinaryDB{
public:

constexpr static uint32_t numberCountries = 249;

struct Header{
  uint32_t numCountries;
  uint32_t numDatasets;
  uint32_t numDataPoints;
};

private:

int debugLevel;

  std::vector<std::string> countryCodes;
  std::vector<uint8_t> continents;
  /*
    Dataset represents one variable for all countries

   */
  
  #include "Dataset.hh"
  
  class Index {
  private:
    std::vector<Dataset> datasets; // index to all variables
  public:
    Index();
    void addDataSet(const Dataset& d);

    uint32_t size() const{
      return datasets.size();
    }

    Dataset& get(uint32_t index){
      return datasets[index];
    }

    Dataset& operator[](uint32_t index){
      return datasets[index];
    }
  };

  Index index;

  std::vector<float> data; // MASSIVE
public:
  GapMinderBinaryDB(const char filename[]);
  void loadDir(const char filename[]); // opendir, readdir, closedir https://man7.org/linux/man-pages/man3/opendir.3.html
  void loadOneFile(const char filename[]);
  /*
    save the file in binary so that it can be loaded in a single memory read next time
  */

  void fill(std::ofstream &f, uint32_t size);

  void saveBinary(const char binaryData[]);

  void loadCountryCodes();

  const Dataset* getDataset(const char dataset[]) const{
    Dataset* d;
    //TODO
    return d;
  }

  float getData(uint32_t countryIndex, uint32_t year, const Dataset* d) const{
    if(numberCountries < countryIndex){
      return NaN;
    }
    if (year < d->countries[countryIndex].startYear ){
      return NaN;
    }
    uint32_t i = d->countries[countryIndex].startIndex + year - d->countries[countryIndex].startYear;
    if (i >= d->countries[countryIndex+1].startIndex){
      return NaN;
    }
    return data[i];
  }

  std::vector<float> getAllDataOneYear(uint32_t year, const Dataset* d){
    std::vector<float> vec;
    if(year < d->startYear){
      vec = {NaN};
      return vec;
    }
    if ( year > d->endYear){
      vec = {NaN};
      return vec;
    }

    for (int i = 0; i < numberCountries; i++){
      uint32_t index = d->countries[i].startIndex + year - d->countries[i].startYear;
      if (i >= d->countries[i+1].startIndex){
        vec.push_back(0xffffffff);
      }else if (year < d->countries[i].startYear){
        vec.push_back(0xffffffff);
      }else{
        vec.push_back(data[index]);
      }
      
    }

    return vec;
  }

  std::vector<float> getAllDataOneCountry(uint32_t countryIndex, const Dataset* d){
    std::vector<float> vec;
    if(numberCountries < countryIndex){
      vec = {NaN};
      return vec;
    }

    for(int i = d->countries[countryIndex].startYear; i < d->countries[countryIndex+1].startYear; i++){
      vec.push_back(data[i]);
    }

    return vec;
  }

};


