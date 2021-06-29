#pragma once

#include <iostream>

class GapMinderBinaryDB {
private:
  std::vector<string> countryCodes;

  /*
    Dataset represents one variable for all countries

   */
  class Dataset {
  private:
    std::string name;    // name of the data set (variable)
    // this might be useful summary data, not required
    uint32_t startYear;  // OPTIONAL starting year (earliest) for this dataset for ALL countries
    uint32_t endYear;    // OPTIONAL end year (latest) for this dataset, ANY country
    uint32_t startIndex; // start index within data
    uint32_t endIndex;   // end index within data (this variable between startIndex,endIndex
    struct Country {
      uint32_t startIndex; // start of the country's data within data
      uint32_t startYear;  // year corresponding to first datapoint. TODO: ASSUMED sequential
    };
    std::vector<Country> countries; 
  public:
    Dataset(uint32_t numCountries,
	    const std::string& name, uint32_t startIndex, uint32_t endIndex,
	    uint32_t startYear = 0, uint32_t endYear = 0) : countries(numCountries) {
      

    }
  };
  
  class Index {
  private:
    std::vector<Dataset> datasets; // index to all variables
  public:
    Index();
    void addDataSet(const std::string& name, uint32_t startIndex, uint32_t endIndex,
	    uint32_t startYear = 0, uint32_t endYear = 0);
  };
  std::vector<Dataset> index;
  std::vector<float> data; // MASSIVE
public:
  GapMinderBinaryDB(const char filename[]) : data(2000000) {
  }
  void loadDir(const char filename[]); // opendir, readdir, closedir https://man7.org/linux/man-pages/man3/opendir.3.html

  /*
    save the file in binary so that it can be loaded in a single memory read next time
  */
  void saveBinary(const char binaryData[]) {
    ofstream f(binaryData, ios::binary);
    //write the index information
    f.write((char*)&data[0], data.size() * sizeof(float)); // write all the numbers
  }

  void loadCountryCodes();
