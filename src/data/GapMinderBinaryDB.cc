#include "data/GapMinderBinaryDB.hh"
#include <fstream>
#include <sstream>
#include "data/BlockLoader.hh"
#include <string>
#include <cstring>

using namespace std;
  
GapMinderBinaryDB::Index::Index(){

}


void GapMinderBinaryDB::Index::addDataSet(const GapMinderBinaryDB::Dataset& d){
  datasets.push_back(d);
}


GapMinderBinaryDB::GapMinderBinaryDB(const char filename[]) : data() {
  data.reserve(2000000);
  loadDir(filename);
}


void GapMinderBinaryDB::loadDir(const char dirName[]){
  {
    ifstream r(string(dirName) + "countryCodes.txt");

    cout << r.is_open() << endl;

    char buffer[4096];
    while(r.getline(buffer, sizeof(buffer))){
      countryCodes.push_back(buffer);
    }
  }

  Dataset d = loadOneFile((string(dirName) + "gdp_per_capita.csv").c_str());

  saveBinary("binaryFile", d);
}

GapMinderBinaryDB::Dataset GapMinderBinaryDB::loadOneFile(const char filename[]){

  Dataset d(countryCodes.size(), filename);

  cout << "inside loadOneFile" << endl;

  ifstream f(filename);
  char buffer[4096];
  char a[4096];
  char b[4096];
  char c[4096];


  int lastYear = 0;
  int currentCountry = 0;

  constexpr uint32_t missing = 0xffffffff; 
  string lastName = "";

  uint32_t index = data.size();
  d.startIndex = index;
  int relativeIndex = 0;
  d.startYear = 3000;
  d.completeStartYear = 0;
  d.endYear = 0;

  while (f.getline(buffer, sizeof(buffer))){

    istringstream line(buffer);
    line.getline(a, sizeof(a), ',');
    line.getline(b, sizeof(b), ',');
    line.getline(c, sizeof(c));
    
    
    if(sizeof(a) != 3){
      //cerr << filename  << " " << relativeIndex << " country code size not 3 " << a << endl;
    }
    if(sizeof(b) != 4){
      //cerr << filename  << " " << relativeIndex << " year size not 4 " << b << endl;
    }

    string name = a;
    uint32_t year = atoi(b);
    float val = atof(c);

    while(name == lastName && lastYear != year - 1){
      cerr << filename  << " " << relativeIndex << " hole in data " << lastYear << " - " << year << endl;
      data.push_back(val);
      lastYear++;
    }

    if (name != lastName){
      d.countries.push_back({index, year});
      currentCountry++;
      if(d.startYear > year){
        d.startYear = year;
      }
      if(d.completeStartYear < year){
        d.completeStartYear = year;
      }
      cout << currentCountry << " " << index << "  " << year << endl;
      lastName = name;
    }else if(name != countryCodes[currentCountry]){
      while(currentCountry < countryCodes.size() && name != countryCodes[currentCountry]){
        d.countries.push_back({missing, missing});
        cout << currentCountry << " missing " << countryCodes[currentCountry] << " " << name << endl;
        currentCountry++;
      }
    }


    data.push_back(val);

    lastYear = year;
    index++;
    relativeIndex++;

    if (d.endYear < year){
      d.endYear = year;
    }
  }

  d.endIndex = index;
  f.close();

  cout << d.endYear << " end year" << endl;
  cout << d.startYear << " start year" << endl;
  cout << d.completeStartYear << " complete start year" << endl;

  return d;
} // opendir, readdir, closedir https://man7.org/linux/man-pages/man3/opendir.3.html

void GapMinderBinaryDB::saveBinary(const char binaryData[], Dataset d) {
    ofstream f(binaryData, ios::binary);

    BlockLoader::GeneralHeader h(BlockLoader::Type::gapminder, 0x0000);
    BlockLoader::SecurityHeaderV0 sec = {0};

    Header header;
    header.numCountries = d.countries.size();
    header.numDataPoints = data.size();
    header.numDatasets = 1;

    f.write((char*)&h, sizeof(BlockLoader::GeneralHeader));
    f.write((char*)&sec, sizeof(BlockLoader::SecurityHeaderV0));
    f.write((char*)&header, sizeof(Header));

    //write the index information
    for(int i = 0; i < d.countries.size(); i++){
      f.write((char*)&d.countries[i].startIndex, sizeof(uint32_t)); 
      f.write((char*)&d.countries[i].startYear, sizeof(uint32_t));
      //cout << d.countries[i].startIndex << " " << d.countries[i].startYear << endl;
    }
    f.write((char*)&data[0], data.size() * sizeof(float)); // write all the numbers
}

void GapMinderBinaryDB::loadCountryCodes(){

}



