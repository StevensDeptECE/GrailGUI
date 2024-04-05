# Design of a Turing-Analyzable Language

Goals

* Turing Analyzable: no halting problem
  * no while loops with arbitrary conditions
  * counting to known n only
  * traversing a list
    * readonly: no modification of the list while in the list!
  * no recursion
  * no mutual recursion
  * all operations to do substantial data processing should be primitives
  * no eval()

```cpp
  fn g() {
     text(100, 100, "hello);

  }
  fn f() {

    // f can't call itself
    // f can only call functions taht are already defined
    g();
  }
``` 

But what about an attacker who changes the OBJECT code
byte interpreter checks run:

```cpp
// normal primitive corresponding to tag
void prim_text(getFloat(), getFloat(), getString()) {}

void prim_text(u32 offset, u32 offset2) {
    float x = getFloat(offset++);
    float y = getFloat(offset++);
    const char* s = getString(offset2++);
    internal_text(x, y, s);

0: 
  PRIM TEXT offset offset2                     // place data in data section at offsets (100,100,"hello" )
  or?  PRIM TEXT 100.0 100.0 offset to "hello"
  RET
1:
  ...

  What about for variables?

x:
  CALL (x-1)
  RET
y:
  PRIM getdata(s, XBF-stream)
  PRIM getmeta(s, XBF-stream)

map<symbol, value>


lookup [loc] --> symbol


buildform:
  ADDCHILD formid TEXTFIELD id
  ADDCHILD 
  form.add(TEXTFIELD)
  form.add()

  DEF FUNC 5 hello
    CALL offset         // offset is a 32-bit positive number > 0. SUBTRACTION ONLY!
  
```