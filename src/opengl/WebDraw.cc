#include "WebDraw.hh"

void processU8() {

}

void processU16() {

}

void processString8(Buffer& metadata, Buffer& data) {
  uint8_t len = data.getU8();
  for (uint8_t i = 0; i < len; i++)
    ; // do something to each byte  data.getU8()


}
void nop() {
  cout << "Unimplemented\n";
}

typedef void (*ProcessDataTypeFunc)();

ProcessDataType processDataType[] = {
                                     processU32,
                                     processU32,
                                     processU32,
                                     processU32,
                                     processU64,
                                     nop,
                                     nop,
                                     

};

void WebDraw::draw(Buffer& metadata, Buffer& data) {
  for DataType t = each byte of metadata
    processDataType[t]();

}
