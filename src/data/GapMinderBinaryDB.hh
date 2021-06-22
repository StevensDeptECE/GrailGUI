#pragma once

#include <iostream>
#include <vector>
#include <string>


constexpr float NaN = 0xffffffff;

class GapMinderBinaryDB {
public:

struct Header{
  uint32_t numCountries;
  uint32_t numDatasets;
  uint32_t numDataPoints;
};

private:
  std::vector<std::string> countryCodes;

  /*
    Dataset represents one variable for all countries

   */
  class Dataset {
  public:
    std::string name;    // name of the data set (variable)
    // this might be useful summary data, not required
    uint32_t startYear;  // OPTIONAL starting year (earliest) for this dataset for ALL countries
    uint32_t completeStartYear; //the latest start year in the database
    uint32_t endYear;    // OPTIONAL end year (latest) for this dataset, ANY country
    uint32_t startIndex; // start index within data
    uint32_t endIndex;   // end index within data (this variable between startIndex,endIndex
    struct Country {
      uint32_t startIndex; // start of the country's data within data
      uint32_t startYear;  // year corresponding to first datapoint. TODO: ASSUMED sequential
    };
    std::vector<Country> countries; 

    Dataset(uint32_t numCountries,const std::string& name) : countries(numCountries) {
      startYear = endYear = startIndex = endIndex = 0;
    }

  };
  
  class Index {
  private:
    std::vector<Dataset> datasets; // index to all variables
  public:
    Index();
    void addDataSet(const Dataset& d);
  };

  Index index;

  std::vector<float> data; // MASSIVE
public:
  GapMinderBinaryDB(const char filename[]);
  void loadDir(const char filename[]); // opendir, readdir, closedir https://man7.org/linux/man-pages/man3/opendir.3.html
  Dataset loadOneFile(const char filename[]);
  /*
    save the file in binary so that it can be loaded in a single memory read next time
  */
  void saveBinary(const char binaryData[], Dataset d);

  void loadCountryCodes();

  const Dataset* getDataset(const char dataset[]) const{
    Dataset* d;
    //TODO
    return d;
  }

  float getData(uint32_t countryIndex, uint32_t year, const Dataset* d) const{
    if(d->countries.size() < countryIndex){
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

    for (int i = 0; i < d->countries.size(); i++){
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
    if(d->countries.size() < countryIndex){
      vec = {NaN};
      return vec;
    }

    for(int i = d->countries[countryIndex].startYear; i < d->countries[countryIndex+1].startYear; i++){
      vec.push_back(data[i]);
    }

    return vec;
  }

};


