#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

inline string change(string data) {
    string result;
    char c = '\0';
    for (int i = 0; i < 8; i++) {
        if (data[i] == '1') c = (c << 1) | 1;
        else c = c << 1;
    }
    // cout<<c;
    result += (unsigned char) c;
    return result;
}


int main(int argc, char **argv) {
    char type[] = "LIST8 1 STRUCT8 5 U128 7 User id STRING8 9 firstName STRING8 8 lastName STRING8 7 address U64 5 phone STRING8 5 email";

    char data[] = "123 Dov Kruger 501 W 2125551212 private@yahoo.com";

    cout << change(data);

    ofstream typeF;
    typeF.open("./type.dat", std::ofstream::binary);
    typeF.write(reinterpret_cast<const char *>(type), sizeof(type));
    typeF.close();
    ofstream dataF;
    dataF.open("./data.dat", std::ofstream::binary);
    dataF.write(reinterpret_cast<const char *>(data), sizeof(data));
    dataF.close();

    ifstream inF;
    inF.open("./data.dat", std::ifstream::binary);
    inF.read(reinterpret_cast<char *>(data), sizeof(data));
    inF.close();

    for (int i = 0; i < sizeof(data); i++) {
        cout << data[i] << endl;
    }

    return 0;

}
