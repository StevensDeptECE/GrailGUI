# Tutorial on Binary Data Transmission and Metadata

Binary data is just bytes. Interpretation is arbitrary.
A receiver could analyze and make a guess, but the only way to know for sure what is there is to encode it.

Example:

```cpp
  uint32_t c = 1;
  uint64_t a = 13, b = 2;
  float f = 1.0;
  double d = 2.0;

  // in ASCII 13 2 1 1.0 2.0

  //downside: we have to parse to get the binary, print to get the ascii
  // cannot tell how long any number is going to be
  // cannot tell whether floating point or integer until you see the .
  // numbers are variable length and inefficient
  // example: 4 byte integer 0 .. 4.2billion   42000000000 is 11 bytes as ASCII 
  // example: 4 byte integer 1 = "1" (only 1 byte) but don't forget the delimiter "1 "

  //upside: no worry about the internal format of numbers
  // uint32_t c = 2;
  // little-endian  02 00 00 00
  // big-endian     00 00 00 02

  // 64-bit
  // little-endian  02 00 00 00 00 00 00 00
  // big-endian     00 00 00 00 00 00 00 02

  // but... little-endian basically won (Intel)
  // ARM emulates intel (can go either way) DEFAULT is  little endian
```


```cpp
    Buffer b(32768); // probably wrong constructor, but create an empty buffer with 32k
    b.write(15); // 15 is int, so write 4 bytes into buffer
    b.write(123ULL); // 64-bit integer, write 8 bytes into buffer
    b.write(1.5f); // write 32-bit float
    b.write(2.0);   // write 64-bit double
```

If the receiver knows what you are sending, they can unpack it

We will assume that everything is little-endian but
Optimal strategy for conversion?
  1. Always convert to big-endian (TCP/IP)  192.168.1.1   192 168 1 1
  2. Receiver makes right. Sender says its endian-ness at start of conversation
     if receiver is the same, do nothing, if not, swap (CORBA)
  3. My invention: client makes right
     I observed that the limiting factor is the server, so for speed, the server should never do anything.
     WE WILL NOT BE DOING THIS BECAUSE EVERYONE IS LITTLE ENDIAN! BUT IF WE HAD TO, THIS IS WHAT WE WOULD DO


Metadata

Suppose the receiver does not know what is coming. See buffer above
```cpp
    Buffer meta(32768); // let's build a buffer of metadata
    meta.write(U32); // 1 byte token saying "32-bit unsigned coming"
    meta.write(U64); // 1 byte token saying 64-bit unsigned integer coming
    meta.write(F32); // 4 byte float
    meta.write(F64); // 8 byte float

    enum Tags {
        U8,          // 0
        U16,         // 1
        U32,         // 1
        U64,
        U128
        U256,
        I8,
        I16,
        ...
        STRING8,       // ASCII Text with a 8-bit length? up to 256 string
        STRING16,       // ASCII Text with a 16-bit length? up to 64k string
        STRING32,       // ASCII Text with a 32-bit length? up to 4B string
        ARRAY,        //  4 byte length(not specified in metadata) and a type
    };

    ARRAY U32

   //    array looks for: length data


    Buffer b2(32768);
    uint32_t a[1024] = {0};
    b2.write(a, 1024); // array will be encoded with length first   00 04 00 00     00 00 00 00, 00 00 00 00, ...

// on receiver side, if you know what is coming:
   uint32_t length = b.getU32();
   for (int i = 0; i < length ; i++)
     cout << b.getU32() ;
```

There are types for arrays and structures


How can a generic client view binary data without knowing what it is?

We need an index.
Suppose we have a server. In grail, we send a 4 byte request:

server request 0

The protocol of the message is extremely simple: send 4 bytes to the server with the request number.
That is the equivalent of a URL

The server should send back metadata describing each page and what it returns
Mapping:

1 ==> ARRAY of StockQuote
2 ==> GEOMAP of US Counties
3 ==> MAP of names to ARRAY of Stockquote

struct Stockquote {
  d : date,        // 32 bit integer (days since Jan 1, 2000)
  high : f32,
  low : f32,      
  open : f32,
  close: f32,
  volume : u64,
}

Define local dictionary for user language

dict.cn
high: "高价"
low:  "低价"
open: "开盘价"
close: "收盘价"
volume: "销售数量"

this is binary. Perhaps we would want a description as well in text

server.request(3)

server sends binary data to client. Client must interpret

Client can display a list of objects as a table

     d             high     low     open    close     volume
     2004-04-01     172.05  169.91  171.82  171.06    1201025151




Your task is to demonstrate updating list(s) on the clietn from the server.
Assuming there is only one, it's easy
  APPEND 03 00 00 00 01 00 00 00 05 00 00 00 03 04 00 00
                         1           5          1027

UPDATE number of things to update
UPDATE 2 0 0 0 2024-04-01 5 0 0 0 hello 3 0 0 0 xyz
UPDATE1 2024-03-42 9 0 0 0 veryLong1

server responds with:
  APPEND1 01 01 00 00                                       Append 1 value to the end = 257 
  APPEND 03 00 00 00 01 00 00 00 05 00 00 00 03 04 00 00    append 3 values 1, 5, 1027
   <name> instructions
   <name> instructions   update the variable on the client wiht this name


Follow stock: AAPL, MSFT, X, IBM

UPDATE "stockquotes" 4 AAPL MSFT X IBM
   AAPL ==> list of apple stock quotes
   MSFT ==> list of MSFT...


Medical application: loading test results, maintaining my local repo of health info? Where is it backed up? Crypto?


FirstName: Dov
Lastname:  Kruger
....

send data in XBF to server


control over local variables? delete, update?

clear local storage

