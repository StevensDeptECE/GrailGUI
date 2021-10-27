#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

    std::ifstream file;
    file.open("secrets.txt", std::ios::in | std::ios::binary);

    if (file.is_open())
        printf("File opened.\n");
    else printf("File failed to open");

    return 0;
}