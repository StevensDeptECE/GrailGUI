#ifdef __cplusplus
extern "C" {
#endif
  void addBuiltin(string name,Datatype t);
  Struct* addStruct(string name);
  void addTypedef(string name, string type);
  void addRegex(string name, string regex);
#ifdef __cplusplus
};
#endif
