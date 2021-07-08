class Dataset {
  public:
    char name[128];    // name of the data set (variable)
    // this might be useful summary data, not required
    uint32_t startYear;  // OPTIONAL starting year (earliest) for this dataset for ALL countries
    uint32_t completeStartYear; //the latest start year in the database
    uint32_t endYear;    // OPTIONAL end year (latest) for this dataset, ANY country
    uint32_t startIndex; // start index within data
    uint32_t endIndex;   // end index within data (this variable between startIndex,endIndex
    uint32_t unused;
    struct Country { // Dataset::Country
      uint32_t startIndex; // start of the country's data within data
      uint32_t startYear;  // year corresponding to first datapoint. TODO: ASSUMED sequential
    };
    Country countries[249];
    Dataset(uint32_t numCountries,const std::string& name) {
      startYear = endYear = startIndex = endIndex = 0;
      strcpy(this->name, name.c_str());
    }

    friend std::ostream & operator<< (std::ostream& s, const Dataset& d){
      s << d.name << std::endl;
      s << d.startYear << std::endl;
      s << d.completeStartYear << std::endl;
      s << d.startIndex << std::endl;
      s << d.endIndex << std::endl;
      
      return s;
    }

  };