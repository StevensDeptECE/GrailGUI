#include <iostream>
#include "util/Buffer.hh"
#include "util/Ex.hh"
using namespace std;

vector<DataType> type; 
vector<string> typeName;


void test0(){
    Buffer t0("test10.bin", 32768, "read");
    int8_t b0 = t0.readI8();
    //float b1 = t0.readFloat();
    int16_t b2 = t0.readI16();
    //double b3 = t0.readDouble();
    cout << b0 << ' '  << b2 <<'\n';
}
/*
void test1() { 
    Buffer t1("test.bin", 32768, "read"); // open buffer for reading
    uint64_t a0 = t1.readUint64(); // 8 bytes
    uint32_t a1 = t1.readUint32(); // 8 bytes
    cout << a0 << ' ' << a1 <<'\n';
}

void test2() {
    Buffer t2("test2.bin", 32768, "read");
    string x = t2.readstring1();
    string y = t2.readstring1();
    cout << x << ' ' << y << '\n';
}*/

/*
void displayText(Buffer &meta, Buffer &data) {
  // read in the metadata
  //read in 1 byte with the code for what comes next
  DataType t = DataType(meta.readUint8());
  switch (t) {
  case DataType::U8:{
    string nameU8 = meta.readstring1();
    uint8_t u8 = data.readUint8();
    cout << nameU8 << " : " << u8 << '\n';
    break;}

  case DataType::U16:{
    string nameU16 = meta.readstring1();
    uint8_t u16 = data.readUint16();
    cout << nameU16 << " : " << u16 << '\n';
    break;}
    
  case DataType::U32:{
    string nameU32 = meta.readstring1();
    uint32_t u32 = data.readUint32();
    cout << nameU32 <<  " : " << u32 << '\n';
    break;}
    
  case DataType::U64:{
    string nameU64 = meta.readstring1();
    uint32_t u64 = data.readUint64();
    cout << nameU64 <<  " : " << u64 << '\n';
    break;}
    
  case DataType::DOUBLE:{
    string nameD = meta.readstring1();
    double db = data.readDouble();
    cout << nameD <<  " : " << db << '\n';
    break;}
    
  case DataType::FLOAT:{
    string nameF = meta.readstring1();
    double fl = data.readFloat();
    cout << nameF <<  " : " << fl << '\n';
    break;}
    
  case DataType::STRING8:{
    string nameS8 = meta.readstring1();
    string s8 = data.readstring1();
    cout << nameS8 <<  " : " << s8 << '\n';
    break;}
    
  case DataType::STRUCT8:{
    // get the number of fields
    // for each field, get the type...
    // call yourself recursively for each member
    string nameST8 = meta.readstring1();
    cout << nameST8 << '\n';
    //    unint8_t repeat = data.readUint8();
    uint8_t num_struct8 = meta.readUint8();
    for (int i = 0; i < num_struct8; i++) {
      displayText(meta,data);
    }
    break;}
    
  case DataType::LIST8:{
    string nameL8 = meta.readstring1();
    cout << nameL8 << '\n';
    
    uint8_t num_list8 = meta.readUint8();
    for (int i = 0; i < num_list8; i++) {
      displayText(meta,data);
    }
    break;}
  }
}
*/
/*
void displayUser(Buffer &buf){
  DataType t = DataType(buf.readUint8());
  
  //type.push_back(t);
  
  switch (t) {
  case DataType::U8:{
    type.push_back(t);
    string nameU8 = buf.readstring1();
    typeName.push_back(nameU8);
    break;}

  case DataType::U16:{
    type.push_back(t);
    string nameU16 = buf.readstring1();
    typeName.push_back(nameU16);
    break;}
    
  case DataType::U32:{
    type.push_back(t);
    string nameU32 = buf.readstring1();
    typeName.push_back(nameU32);
    break;}
    
  case DataType::U64:{
    type.push_back(t);
    string nameU64 = buf.readstring1();
    typeName.push_back(nameU64);
    break;}
    
  case DataType::DOUBLE:{
    type.push_back(t);
    string nameD = buf.readstring1();
    typeName.push_back(nameD);
    break;}
    
  case DataType::FLOAT:{
    type.push_back(t);
    string nameF = buf.readstring1();
    typeName.push_back(nameF);
    break;}
    
  case DataType::STRING8:{
    type.push_back(t);
    string nameS8 = buf.readstring1();
    typeName.push_back(nameS8);
    break;}
    
  case DataType::STRUCT8:{
    type.push_back(t);
    string nameST8 = buf.readstring1();
    typeName.push_back(nameST8);
    
    uint8_t num_struct8 = buf.readUint8();
    for (int i = 0; i < num_struct8; i++) {
      displayUser(buf);
    }
    break;}
    
  case DataType::LIST8:{
    //string nameL8 = data.readstring1();
    //cout << nameL8 << '\n';
    
    uint8_t num_list8 = buf.readUint8();
    displayUser(buf);
    
    for (int i = 0; i < num_list8; i++) {
      for (int j = 0; j < type.size(); j++) {
	
    	DataType t = type[j];
	cout << typeName[j] << "  ";
	
	switch (t) {
	case DataType::U8:{
	  uint32_t u8 = buf.readUint8();
	  cout << " : " << u8 << '\n';
	  break;}
	  
	case DataType::U16:{
	  uint8_t u16 = buf.readUint16();
	  cout << " : " << u16 << '\n';
	  break;}
	  
	case DataType::U32:{
	  uint32_t u32 = buf.readUint32();
	  cout << " : " << u32 << '\n';
	  break;}
	  
	case DataType::U64:{
	  uint32_t u64 = buf.readUint64();
	  cout << " : " << u64 << '\n';
	  break;}

	case DataType::DOUBLE:{
	  double db = buf.readDouble();
	  cout << " : " << db << '\n';
	  break;}
	  
	case DataType::FLOAT:{
	  float fl = buf.readFloat();
	  cout << " : " << fl << '\n';
	  break;}
		  
	case DataType::STRING8:{
	  string s8 = buf.readstring1();
	  cout << " : " << s8 << '\n';
	  break;}

	case DataType::STRUCT8:{
	  string s8 = " ";
	  cout << s8 << '\n';
	  break;}
	}
      }
      cout << '\n';
    }
    break;}
  }
}
*/
void test() {
  test0();
  /* Buffer t3("test3meta.bin", 32768, "read");
     Buffer t3b("test3.bin", 32768, "read");
     displayText(t3, t3b);*/
  /* 
  Buffer metadata("metadata.bin", 32768, "read");
  Buffer data("data.bin", 32768, "read");
  displayText(metadata, data);
  */
  // Buffer userdata("teststudent.bin", 32768, "read");
  //displayUser(userdata);
}

int main(int argc, char **argv) {
	try {
		test();
	} catch (Ex& e) {
		cerr << e << '\n';
	}
}
