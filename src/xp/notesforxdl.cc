/*
notes on XDL transport

*/

#if 0
  class Student : public XDLType {
    public:
    string firstName, lastName;
    uint32_t age;

    void write(Buffer& out) {
      out.writeString(firstName);
      out.writeString(lastName);
      out.write(age);
    }
    void writeMeta(Buffer& out) {
       out.write(DataType::STRUCT8);
       out.writeString("Student");
       out.write(uint8_t(3));

    }
  }

template<typename T>  class List<T> : public DynArray, public XDLType {
  public:
    List(std::string& name) : XDLType(name) {}
    void write(Buffer& out) {
      out.write(size);
      for (int i = 0; i < size; i++)
        out.write(data[i]);

    }
    void writeMeta(Buffer& out) {
      out.write(DataType::List8);
      out.write(name);
      writeMeta(T);

    }
};
#endif