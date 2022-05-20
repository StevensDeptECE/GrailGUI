#include "util/Buffer.hh"
#include "xdl/std.hh"

using namespace std;

int main() {
  /*
  Buffer buf("teststudent.bin", 32768);
  UserId userid(12345);
  Address address("60 Webster Ave");
  Phone phone(5512635567);
  Email email("lmao4@stevens.edu");
  User student1(userid, "Luoyuan", "Mao", address, phone, email);
  // student1.writeMeta(buf);
  //student1.write(buf);

  List<User> students;
  students.add(student1);

  UserId userid2(223456);
  Address address2("1313 Mockingbird Lanee");
  Phone phone2(7123351567);
  Email email2("wcao9@stevens.edu");
  User student2(userid2,  "Wanying", "Cao", address2, phone2, email2);
  students.add(student2);
  //  students.add(223456, "Wanying", "Cao", "1313 Mockingbird Lanee",
  7123351567, "wcao9@stevens.edu");

  students.writeMeta(buf);
  students.write(buf);
  //  students.add(223456, "Wanying", "Cao", "1313 Mockingbird Lanee",
  7123351567, "wcao9@stevens.edu");

  */
  /*
  Buffer buf("test.bin", 32768); // open buffer for write
  uint64_t a = 123455;
  uint32_t b = 13;
  buf.write(a);
  buf.write(b);
  Buffer buf2("test2.bin", 32768); // open buffer for write
  string x = "testing";
  string y = "hello";
  buf2.write(x);
  buf2.write(y);
  */
  Buffer t0("test10.bin", 32768);
  int8_t aa = -100;
  int16_t ab = 22345;
  //  float ac=123.45;
  //  double ad=456.78;
  // uint16_t ac=77;
  t0.write(aa);
  t0.write(ab);
  // t0.write(ac);
  // t0.write(ad);
  /*
  Buffer buf3("test3meta.bin", 32768); // open buffer for write
  buf3.write( uint8_t(DataType::STRUCT8) ); // write the metadata for struct8 as
  one byte string structname = "info"; buf3.write( structname); //struct have a
  name buf3.write( uint8_t(2) ); // write the number of fields buf3.write(
  uint8_t(DataType::STRING8) ); string fieldname = "name";
  buf3.write(fieldname); // write first the one byte length and then the string
  (4name)

  buf3.write( uint8_t(DataType::U32) );
  fieldname = "age";
  buf3.write(fieldname); // write first the one byte length and then the string
  (3age) Buffer buf3b("test3.bin", 32768); // open data for the above metadata
  //uint8_t repeat = 2;
  string name = "Dov";
  uint32_t age = 52;
  //string name1 = "mao";
  //uint32_t age1 = 24;
  //buf3b.write(repeat);// add repeat time
  buf3b.write(name);
  buf3b.write(age);
  //buf3b.write(name1);
  //buf3b.write(age1);*/
}
