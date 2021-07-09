#include "data/GapMinderBinaryDB.hh"
#include <fstream>
#include <sstream>
#include "data/BlockLoader.hh"
#include <string>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <unordered_map>

using namespace std;
  
GapMinderBinaryDB::Index::Index(){

}


void GapMinderBinaryDB::Index::addDataSet(const GapMinderBinaryDB::Dataset& d){
  datasets.push_back(d);
}


GapMinderBinaryDB::GapMinderBinaryDB(const char filename[]) : data() {
  debugLevel = 0;
  data.reserve(2000000);
  loadDir(filename);
}


void GapMinderBinaryDB::loadDir(const char dirName[]){

  chdir(dirName);

  ifstream c("countryContinent.txt");

  char buffer[4096];
  char a[8];
  char b[8];

  unordered_map <string, uint8_t> continentNames = {
    {"na", 0}, {"eu", 1}, {"as", 2}, {"sa", 3}, {"af", 4}, {"oc", 5}, {"an", 6}
  };


  while(c.getline(buffer, sizeof(buffer))){
    istringstream line(buffer);
    line  >> a >> b;

    continents.push_back(continentNames[b]);
    countryCodes.push_back(a);

  }

  cout << countryCodes.size() << endl;
  cout << continents.size() << endl;


  DIR *pDIR;
  struct dirent *entry;


  chdir("allFiles");
  if( pDIR=opendir(".")){
    while( (entry = readdir(pDIR)) != nullptr ){
      if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 ){
        if(strcmp(entry->d_name, "gdp_per_capita.csv") == 0){
          cout << entry->d_name << endl;
        }
        cout << entry->d_name << endl;
        loadOneFile(entry->d_name);      
      }
    }
  }
  chdir("..");

  saveBinary("GapMinderDBFile");
}

void GapMinderBinaryDB::loadOneFile(const char filename[]){

  Dataset d(countryCodes.size(), filename);


  if(debugLevel >= 1){
    cout << "reading " << filename << endl;
  }

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


  f.getline(buffer, sizeof(buffer));
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
    int dumb = 0;
    if(name == "chn" && year == 2019){
      dumb++;
    }
    float val = atof(c);

    if(name == lastName && year < lastYear){
      cout << "time discontinuity " << filename << " " << name << endl;

    }else{
      while(name == lastName && lastYear != year - 1){
        if (debugLevel >= 2){
          cerr << filename  << " " << relativeIndex << " hole in data " << lastYear << " - " << year << endl;
        }
        data.push_back(val);
        lastYear++;
      }
    }

    if (name != lastName){
      d.countries[currentCountry] = Dataset::Country({index, year});
      currentCountry++;
      if(d.startYear > year){
        d.startYear = year;
      }
      if(d.completeStartYear < year){
        d.completeStartYear = year;
      }
      if(debugLevel >=1){
        cout << currentCountry << " " << index << "  " << year << endl;
      }
    
      lastName = name;
    }else if(currentCountry < countryCodes.size() && name != countryCodes[currentCountry] && name != lastName){
      while(currentCountry < countryCodes.size() && name != countryCodes[currentCountry]){
        d.countries[currentCountry] = Dataset::Country({missing, missing});
        if(debugLevel >= 1){
          cout << currentCountry << " missing " << countryCodes[currentCountry] << " " << name << endl;
        }
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

  if(debugLevel >= 1){
    cout << d.endYear << " end year" << endl;
    cout << d.startYear << " start year" << endl;
    cout << d.completeStartYear << " complete start year" << endl;
  }

  this->index.addDataSet(d);
} // opendir, readdir, closedir https://man7.org/linux/man-pages/man3/opendir.3.html

void GapMinderBinaryDB::fill(ofstream &f, uint32_t size){
  const static char filler[8] = {0};
  if (size%8 != 0){
    int mod = size%8;
    f.write((char*)&filler, 8-mod);
  }

}

void GapMinderBinaryDB::saveBinary(const char binaryData[]) {

    ofstream f(binaryData, ios::binary);

    BlockLoader::GeneralHeader h(BlockLoader::Type::gapminder, 0x0000);
    BlockLoader::SecurityHeaderV0 sec = {0};
  

    Header header;
    header.numCountries = countryCodes.size();
    header.numDataPoints = data.size();
    header.numDatasets = this->index.size();

    f.write((char*)&h, sizeof(BlockLoader::GeneralHeader));
    f.write((char*)&sec, sizeof(BlockLoader::SecurityHeaderV0));
    f.write((char*)&header, sizeof(Header));

    fill(f, (sizeof(BlockLoader::GeneralHeader)+sizeof(BlockLoader::SecurityHeaderV0)+
        sizeof(Header)));

    for (int i = 0; i < countryCodes.size(); i++){
      f.write(countryCodes[i].c_str(), 3);
    }

    fill(f, countryCodes.size()*3);
    
    f.write((char*)&continents[0], continents.size());

    fill(f, continents.size());

    //write the index information
    for (int j = 0; j < index.size(); j++){
      const Dataset& d = index[j]; 

      f.write((char*)& d, sizeof(d));
      cout << d << endl;
      
    }

    fill(f, (index.size()*header.numCountries*sizeof(uint32_t)));

    f.write((char*)&data[0], data.size() * sizeof(float)); // write all the numbers

    fill(f, (data.size()*sizeof(float)));
}


void GapMinderBinaryDB::loadCountryCodes(){

}



