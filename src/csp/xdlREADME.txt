struct Point {
  F64 x,y,z;
}

typedef PointList  List32<Point>;


LIST32 length (4 bytes) STRUCT8 3 5 point

1124124.12410 12142.12410 31241.124120
2.5 1.0 -1.2
1.1243124512 2.8 -31241245.1241
...

server: data in binary ---> ASCII sends to client --> binary computes --> draws to screen (turns into ascii Again)
x x x x x x x x y y y y y y y y z z z z z z z z
x x x x x x x x y y y y y y y y z z z z z z z z
...
JSON: about 3x bigger
[
 {"x": 1124124.12410, "y": 12142.12410, "z": 31241.124120},
  {"x": 1124124.12410, "y": 12142.12410, "z": 31241.124120},
   {"x": 1124124.12410, "y": 12142.12410, "z": 31241.124120}
]

XML: 5 x bigger
<listofpoints>
<point><x>1124124.12410</x><y>12142.12410</y><z>31241.124120</z></point>


Grail+XDL

1. Local storage: User defines how much storage this app is allowed
2. predownload the history of data ONCE
3. evey day, download the delta.

4. even intra day, you do not have to load everything
  a. prebuilt binary of all APPL data up to today, pre-compressed
    for a data set like medical (temperature, blood pressure, etc or stock
    numbers are not random
    98.6 99.0 98.7 ...


struct Health {
  real(3.1) temp; // 98.6 3 digits 1 after decimal?
  deltaencode u8        systolicbp; //120/60
  deltaencode u8        diastolicbp;
}





  b. load the delta





Presentation of XDL
Send binary data

Generic client that can render XDL data.


struct Point {
  F64 x,y,z;
}
1 2 3

<table>
<tr><td>x</td><td>y</td><td>z</td></tr>
<tr><td>1</td><td>2.5</td><td>3.2</td></tr>

</table>




struct Health {
  real(3.1) temp; // 98.6 3 digits 1 after decimal?
  deltaencode u8        systolicbp; //120/60
  deltaencode u8        diastolicbp;
}

en.dict
Health{
  temp         "Temperature (F)"
  systolicbp   "Systolic Blood Pressure"
  diastolicbp  "Diastolic Blood Pressure"
}

cn.dict
Health{
  temp         ""
  systolicbp   "Systolic Blood Pressure"
  diastolicbp  "Diastolic Blood Pressure"
}


temp		98.6
systolicbp	120
diastolicbp	60


Temperature(F)		98.6
Systolic Blood Pressure	120
diastolicbp	60


We are using TCP/IP send socket --> get acknowledge

Google QUIC protocol uses UDP sockets
