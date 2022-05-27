#include <iostream>

#include "util/Buffer.hh"

using namespace std;

vector<DataType> type;
vector<string> typeName;

void displayUser(Buffer &buf) {
  DataType t = DataType(buf.readU8());

  // type.push_back(t);

  switch (t) {
    case DataType::U8: {
      string nameU8 = buf.readString8();
      uint8_t u8 = buf.readU8();
      cout << nameU8 << " : " << u8 << '\n';
      break;
    }

    case DataType::U16: {
      string nameU16 = buf.readString8();
      uint16_t u16 = buf.readU16();
      cout << nameU16 << " : " << u16 << '\n';
      break;
    }

    case DataType::U32: {
      string nameU32 = buf.readString8();
      uint32_t u32 = buf.readU32();
      cout << nameU32 << " : " << u32 << '\n';
      break;
    }

    case DataType::U64: {
      type.push_back(t);
      string nameU64 = buf.readString8();
      typeName.push_back(nameU64);
      // uint32_t u64 = buf.readUint64();
      //  cout << nameU64 <<  " : " << u64 << '\n';
      break;
    }

    case DataType::F64: {
      string nameD = buf.readString8();
      double db = buf.readF64();
      cout << nameD << " : " << db << '\n';
      break;
    }

    case DataType::F32: {
      string nameF = buf.readString8();
      typeName.push_back(nameF);
      float fl = buf.readF32();
      cout << nameF << " : " << fl << '\n';
      break;
    }

    case DataType::STRING8: {
      type.push_back(t);
      string nameS8 = buf.readString8();
      typeName.push_back(nameS8);
      // string s8 = buf.readstring1();
      //  cout << nameS8 <<  " : " << s8 << '\n';
      break;
    }

    case DataType::STRUCT8: {
      type.push_back(t);
      string nameST8 = buf.readString8();
      typeName.push_back(nameST8);
      // cout << nameST8 << '\n';
      // typeName.push_back(nameST8);
      uint8_t num_struct8 = buf.readU8();
      for (int i = 0; i < num_struct8; i++) {
        displayUser(buf);
      }
      break;
    }

    case DataType::LIST8: {
      // string nameL8 = data.readstring1();
      // cout << nameL8 << '\n';

      uint8_t num_list8 = buf.readU8();
      displayUser(buf);

      for (int i = 0; i < num_list8; i++) {
        for (int j = 0; j < type.size(); j++) {
          DataType t = type[j];
          cout << typeName[j] << "  ";

          switch (t) {
            case DataType::U64: {
              uint32_t u64 = buf.readU64();
              cout << " : " << u64 << '\n';
              break;
            }

            case DataType::STRING8: {
              string s8 = buf.readString8();
              cout << " : " << s8 << '\n';
              break;
            }

            case DataType::STRUCT8: {
              string s8 = " ";
              cout << s8 << '\n';
              break;
            }
          }
        }
        cout << '\n';
      }
      break;
    }
  }
}

int main(int argc, char **argv) {
  Buffer userdata("teststudent.bin", 32768, "read");
  displayUser(userdata);
}
