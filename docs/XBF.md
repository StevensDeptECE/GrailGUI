# XBF Definition

XBF (eXtensible Binary Format) is a data standard based on the original concept
of IDL, the data standard for CORBA, and protocols like Google protobuf.
XBF supports key features not in CORBA, protobuf, or any other protocol.

CORBA IDL primarily defined interfaces, method calls that could be invoked remotely.
It also defined structures representing compound data types that could be sent between computers.
CORBA was designed to use "receiver makes right" on the premise that if the server is little-endian,
and the client is little-endian, then nothing needs to be done.
XBF has a better concept: client makes right. The server is the side that is busy. It is shared and in
contention, so the best throughput results from asking the server to do the least.
However, since most computers today are little-endian, we have ignored this for now. Everything is
simply little-endian.

Google protobuf supports definition of data and automatically generates custom code on either side to transmit and receive the data.

XBF extends the concepts in these two APIs to include:
* More data types
* Metadata to describe the meaning of a binary stream
* Metadata allows a client to know what the data is and process in generic ways without compilation
  * Examples of metadata use include displaying tables of data with numeric formatting
  * Graphing data
  * It should be possible to display data on a web client without knowing what the data is.
  * Translation of variable names
     - It should be possible to display data with an explanation in the local language without programming
  * Internal field names in XBF are not merely for programmers because they can be used to display column names in a local language
  * Translation dictionaries are supported to local languages, and should be automatically generated using AI, but reviewed by humans until AI is sufficiently good.



1. Support for more integer data sizes up to 256 bits to support cryptographic primitives among others
2. Formal specification of Date types for transport of dates between computers and languages.
3. Bit vectors to support efficient transport of compact data without requiring custom encoding.


5. In order to reduce repetition, XBF includes the notion of functions and subroutines that can be defined compactly. The language is restricted for safety
      * All code in XBF is Turing analyzable. That is, halting is guaranteed and the cost of the code can be computed at load time.
      * In order to achieve this, there are only constant iteration loops, and loops over a list. The number of iterations is always known at program load time.
      *  Functions may call other functions, but recursion is forbidden.
      * Mutual recursion is impossible because functions may only call previously defined functions.
      * The resulting language is safe but can be used to reduce the complexity of complex data containing patterns.

## Binary Tokens

All tokens in XBF are 1-byte. The following list shows each token and its corresponding value.


// TODO: need to add metadata F32 and F64

| Tag | Val | Tag | Val | Tag | Val |
| --- | --- | --- | --- | --- | --- |
| U8 | 00 | I8 | 07 | DATE | 0E |
|U16|01|I16|08|TIME|0F|
|U24|02|I24|09|DATETIME|10|
|U32|03|I32|0A|CALENDAR|11|
|U64|04|I64|0B|STRING8|12|
|U128|05|I128|0C|STRING16|13|
|U256|06|I256|0D|STRING32|14|
|  
|LIST8|15|DEF|21|LOOP|28|
|LIST16|16|STRUCT|22|FOREACH|29|
|LIST32|17|FUNC|23|IF|2A|
|MAP|18|LIBCALL|24|SET|2B|
|COPYLIST|19|CALL|25|VAR|2C|
|SYNCMAP|20|LCALL|26|UPDATE1| 2D |


List of UPDATE operations
| Tag      | Val | Tag | Val | Tag | Val |
| ---      | --- | --- | --- | --- | --- |
| APPEND8  | 00  |     |     |     |     |
| REM_END8 | 01  |     |     |     |     |
| INSERT8  | 02  |     |     |     |     |


### Examples

We assume at present that structures have a fieldlength of 8 bits.
All named types have names with a length of 8 bits

| c++                    | XBF Metadata | Hex Metadata |XBF Data |
| ---                    | ---          | ---          | ---     |
| uint64_t a = 1;        | U64          | 04           |01 00 00 00 00 00 00 00 |
| List<uint32_t> b = {5,7};      | LIST16 U32   | 16 03        |02 00 05 00 00 00 07 00 00 00 |
| class A { int x,y; } A a1(16,512);          | STRUCT 01 A 02 U32 U32 | 22 01 41 02 03 03 | 10 00 00 00 00 02 00 00 |
| client.copyLocal("xyz", b) | COPYLIST LIST16 U32 03 x y z 02 10 00 00 00 00 02 00 00 | 19 16 03 ... | |
| b.append(11); b.update("xyz"); | UPDATE1 03 x y z APPEND 01 0B 00 00 00 |
| b.removeEnd(2); b.update("xyz"); | UPDATE1 03 x y z REMOVEEND 02 | only the 5 remains |

Equivalent of some of the above in JSON
```json
{
  "x": 16,
  "y": 512
}
```
### Data Management

MSGSEND     send XBF to other
MSGRECV     wait to receive XBF
STORE       save to local storage
UPLOAD      load changes from local storage back to server

### Scope Primitives

PAGE  lasts only while this page is loaded
SESSION     lasts as long as the user stays logged in
APP         lasts as long as the app is installed
MANUAL      lasts until the user deletes it

### Type Modifiers

When declaring types, XBF allows optional modifiers that describe the packing and compression. Each field can be declared encoded in various ways.

* DELTA (delta encoded)
* DISCRETIZED16 (for floating point values)

## Metadata Examples

All metadata examples are described in Rust notation.

```rust
struct Point {
  x: f64,
  y: f64,
  z: f64,
}

Vec<u32>

Vec<Point>

fn build_1(x: f32, y: f32, s: string) {
      rect(x,y,100,100);
      ellipse(x,y,100,100);
      text(x,y,s);
}

fn build_2(x1: f32, x2: f32, y1: f32, y2: f32,
      s1: string, s2: string, s3: string, s4: string) {
      build_1(x1, y1, s1);
      build_1(x2, y1, s2);
      build_1(x1, y2, s3);
      build_1(x2, y2, s4);
}
```
TODO: Should there be a fixed length list (Array?) with the 
length in the metadata?
```
[DEF] 01 [STRUCT] 05 P o i n t 03 F64 1 x F64 1 y F64 1 z
[LIST] 03 00 00 00
point is defined as type=256, first user-defined type
[LIST] 00 01 00 00

[FUNC] 03 03 F32 F32 STRING
  LIBCALL 01 00  [00] (x)
  LIBCALL 02 00  [01] (y)
  LIBCALL 03 00  [02] (s)

[FUNC] 08 03 REPT 04 F32 REPT 04 STRING8
  CALL 01 00 VAR 00 02 04     (call build_1(x1,y1,s1)
  CALL 01 00 VAR 01 02 05     (call build_1(x2,y1,s2)
  CALL 01 00 VAR 00 03 06     (call build_1(x1,y2,s3)
  CALL 01 00 VAR 01 03 07     (call build_1(x2,y2,s4)

server: List<int,"Fred"> [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
client: List<int, "Fred"> [ 2, 3, 5, 6, 7]
[SYNCLIST] 04 Fred [03] [01 00 00 00] [09 00 00 00] [0A 00 00 00]
   [ADDSTART] [01] [ADDEND] [02]

question: How to incorporate deletion?
// for inserting and deleting start and end, O(n)
fn update_list(new_size: u32,
    delta: Vec<T>, add_start: u32, add_end: u32, delRanges : ??) {
      if size + add_start + add_end - remove_start - remove_end > capacity {
            old = m;
            m = new T[newSize];
      } else {
            old = m;
      }
      if 
      for i : u32 = 0; i < add_start, i++ {
            m[i] = delta[i];
      }
      for i : u32 = del_start; i < size - del_end; i++ {
            m[add_start - del_start + i] = orig[i];
      }
      for i : u32 = 0; i < size(delta); i++ {
        m[add_start + (size - del_end) - del_start] = delta[i];
      }
}

```
1. There is a formal binary metadata language that is standard across
all platforms. For any object defined in XDL, there is a sequence of
bytes defining the metadata that tells any reader the sequence of
types to be expected. Because of this feature, it is possible to write
a generic client that can read data from any server. There are
therefore two ways to send data in XDL. The first way is the same as
IDL and Protobuf -- the programmer defines data in XDL, generates
custom code for server and client, and each sends the data, knowing
that the other side knows at compile time what to expect. The second
is to generate data, send the metadata either before or every time the
data is sent, and let the client interpret the metadata to discover

1. XDL includes the concept of localization of names of
variables. Data in XDL is designed to share and be viewed by the other
party. It is not just a programmer internal detail. Accordingly, there
is a formal specification of how to display data including the name of
the variables, which can be translated into local languages.

# Binary Download of Code and Deltas

Suppose we have code on the server. It must be downloaded to the client

index.grail -->         0 = index.grb
getstudents.grail -->   1 = getstudents.grb

LMS.lzma    is a compressed file containing each .grb file
download

suppose later, index.grail is updated, changing index.grb
binary delta algorithms (patch)