#include "util/datatype.hh"

const char* DataTypeNames[] = {

  "u8", "u16",	"u24", "u32", "u64", "u128", "u256",
  "i8", "i16",	"i24", "i32", "i64", "i128", "i256",
  "f32", "f64",
	"bool",
	"date",	"juldate", "timestamp",
	"string8", "string16", "string32", "string64",
	"utf8_8", "utf8_16", "utf8_32", "utf8_64",
	"utf16_8", "utf16_16", "utf16_32", "utf16_64",
  "regex",
  
	"list8", "list16", "list32", "list64",
  "struct8", "struct16", "struct32",
	
	"dynamiclist1",	"dynamiclist2",
	"func1", "func2",
	"funcparam1", "funcparam2",
	"loop1", "loop2",	"loop4",
	"bitvec1", "bitvec2", "bitvec4", "bitvec8", "bitvec32", "bitvec64",
	"bitfields",
	"arithmetic",
	"jpeg",	"jpeglist",	"flif",	"fliflist",
	"blob4", "blob8",
	"student",
	"bigint",
  "enum_size does not really exist"
};

unordered_map<string,DataType> mapnames;
void loadmap(){
	for (int i = 0; i < int(DataType::ENUM_SIZE); i++)
		mapnames[DataTypeNames[i]] = (DataType)i;
}
