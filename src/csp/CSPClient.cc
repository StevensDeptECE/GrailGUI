//
// Created by maquiles on 11/13/17.
//

#include "csp/CSPClient.hh"

#include <chrono>

using namespace std;

CSPClient::CSPClient(char* addr, int port)
    : s(addr, port), pages(vector<Page>()), in(s.getIn()), out(s.getOut()) {}

// Switch statement for reading in dynamic data from buffer
void CSPClient::addDynamic(const DataType& type, char*& arrayp) {
  switch (type) {
    case UINT1:
      arrayp += sprintf(arrayp, "%u", in.read<uint8_t>());
      break;
    case UINT2:
      arrayp += sprintf(arrayp, "%u", in.read<uint16_t>());
      break;
    case UINT4:
      arrayp += sprintf(arrayp, "%u", in.read<uint32_t>());
      break;
    case UINT8:
      arrayp += sprintf(arrayp, "%lu", in.read<uint64_t>());
      break;
    case INT1:
      arrayp += sprintf(arrayp, "%d", in.read<int8_t>());
      break;
    case INT2:
      arrayp += sprintf(arrayp, "%d", in.read<int16_t>());
      break;
    case INT4:
      arrayp += sprintf(arrayp, "%d", in.read<int32_t>());
      break;
    case INT8:
      arrayp += sprintf(arrayp, "%ld", in.read<int64_t>());
      break;
    case STRING1:
      in.copyData(arrayp, type);  // Is this faster than using in.read<char>?
      break;
    case STRING4:
      in.copyData(arrayp, type);
      break;
    case LIST1:
      for (int i = 0; i < 25; i++) {
        arrayp += sprintf(arrayp, "%u", in.read<uint16_t>());
      }
      break;
    case LIST2:
      for (int i = 0; i < 500; i++) {
        arrayp += sprintf(arrayp, "%u", in.read<uint16_t>());
      }
      break;
    case FLOAT:
      arrayp += sprintf(arrayp, "%f", in.read<float>());
      break;
    case DOUBLE:
      arrayp += sprintf(arrayp, "%f", in.read<double>());
      break;
  }
}

// Page Request if static content is already loaded into memory
pageBuffer CSPClient::pageRequest(
    uint32_t pageid) {  // Request dynamic data from server
  if (pageid >= pages.size()) {
    cerr << "Page not in cache";
  } else {
    if (pages[pageid]
            .isStatic())  // If no dynamic content, return static content
      return {pages[pageid].getStaticPointer(), pages[pageid].getSize()};

    // If there is dynamic content ask the server for it:
    char* staticContent = pages[pageid].getStaticPointer();
    vector<Insertion> insertions = pages[pageid].getInsertions();
    s.send(pageid);
    // Set up pointer arrayp to move through initial array rather than newCount
    char* tempArray =
        new char[pages[pageid].getSize() + 1];  // Need size + 1 to accomodate
                                                // for \0 added on from sprintf.
    char* arrayp = tempArray;
    uint32_t staticCount = 0;
    // Loop through insertions copying over data
    for (auto i : insertions) {
      uint32_t offset = i.offset;
      DataType type = i.d;
      memcpy(arrayp, staticContent,
             offset - staticCount);  // Copy over static data up to offset
      arrayp += offset - staticCount;
      staticContent += offset - staticCount;
      staticCount += offset - staticCount;  // Increment pointers

      // Copy over dynamic data
      addDynamic(type, arrayp);
      // in.copyData(tempArray + newCount, type, newCount);
    }
    memcpy(arrayp, staticContent,
           pages[pageid].getStaticSize() -
               staticCount);  // Finish copying static data
    tempArray[pages[pageid].getSize()] =
        '\0';  // Set last value of char array to be \0 for printing - can be
               // removed
    return {tempArray,
            pages[pageid].getSize() + 1};  // Return pageBuffer of result
  }
}

// PageRequestFile is not currently relevant.
// Combines static and dynamic content from a file stored in staticContent
pageBuffer CSPClient::pageRequestFile(uint32_t pageid) {
  if (pageid >= pages.size()) {
    cerr << "Page not in cache";
  } else {
    // Get Insertions and file name
    vector<Insertion> insertions = pages[pageid].getInsertions();
    string filename = pages[pageid].getStaticContent();
    // Open file to read
    FILE* inFile = fopen(filename.c_str(), "r");
    char* tempArray = new char[pages[pageid].getSize()];
    char* array_p = tempArray;  // Initialize pointer for new array
    uint32_t count = 0;
    s.send(pageid);  // Send ID to the server
    // Loop through insertions, copy over static data and read in dynamic data
    // from the buffer
    for (auto i : insertions) {
      fread(array_p, 1, i.offset - count, inFile);
      array_p += i.offset - count;
      count += i.offset - count;
      addDynamic(i.d, array_p);
    }
    // Finish copying over static content
    fread(array_p, 1, pages[pageid].getStaticSize() - count + 2, inFile);
    fclose(inFile);
    return {tempArray, pages[pageid].getSize()};  // Create PageBuffer object
                                                  // with file data and size
    // lseek maybe file.seek
  }
}

// testPrint should not be relevant anymore
void CSPClient::testPrint(int pageid) {
  s.send(pageid);
  // Buffer& in = s.getIn();
  switch (pageid) {
    case 0: {
      set1 temp = in.read<set1>();
      dump1(temp);
      break;
    }
    case 1: {
      for (uint32_t i = 0; i < 10; i++) {
        cout << in.read<uint32_t>() << endl;
      }
      break;
    }
    case 2: {
      int l1 = (int)in.read<uint8_t>();
      for (int i = 0; i < l1; i++) {
        cout << in.read<char>();
      }
      cout << endl;
      l1 = (int)in.read<uint8_t>();
      for (int i = 0; i < l1; i++) {
        cout << in.read<char>();
      }
      cout << endl;
      // TODO: Need a way to know when @ end of buffer
      break;
    }
    case 3: {
      set4 temp = in.read<set4>();
      dump4(temp);
      break;
    }
    case 4: {
      List1<uint32_t> temp = in.readList<uint32_t>();  // <List1<uint32_t>>();
      // cout<<"Used: "<<temp.getUsed()<<endl;
      // cout<<"Capacity: "<<temp.getCapacity()<<endl;
      // cout<<"Slot 0: "<<temp[0]<<endl;
      // in.read<uint32_t>();
      for (uint8_t i = 0; i < 15; i++) {
        // cout<<"Expected: "<<(int)in.read<uint32_t>()<<"\t\tActual:
        // "<<(int)temp[i]<<endl; cout<<in.read<uint32_t>()<<endl;

        // cout<<temp[i]<<endl;
        cout << temp.getData(i) << endl;
      }
      // cout<<sizeof(in)<<endl;

      break;
    }
    case 5: {
      // int l1=(int)in.read<uint32_t >();
      /*int id=(int)in.read<uint32_t >();
      int l1=(int)in.read<uint32_t >();
      int l2=(int)in.read<uint32_t >();
      cout<<"ID: "<<id<<endl;
      cout<<"Length of First: "<<l1<<endl;
      cout<<"Length of Last: "<<l2<<endl;
      for(int i=0;i<l1;i++){
          cout<<in.read<char>();
      }
      cout<<endl;
      for(int i=0;i<l2;i++){
          cout<<in.read<char>();
      }
      cout<<endl;*/
      Student temp = in.readStudent();
      cout << temp.getFirst() << endl;
      cout << temp.getLast() << endl;
      cout << (int)temp.getID() << endl;
      break;
    }
    case 6: {
      // List1<Student> temp=in.readList<Student>();
      // Student item1 = temp.getData(0);
      // cout<<temp.getUsed()<<endl;
      // cout<<"empty"<<endl;
      List1<Student> temp = in.readStudentList();
      // Student item1 = temp.getData(0);
      // cout<<temp.getUsed()<<endl;
      // cout<<"item1: "<<item1.getFirst()<<endl;
      cout << "item1: " << temp.getData(0).getFirst() << endl;
      cout << "item1: " << temp.getData(0).getLast() << endl;
      cout << "item1: " << temp.getData(0).getID() << endl;
      cout << endl;
      cout << "item2: " << temp.getData(1).getFirst() << endl;
      cout << "item2: " << temp.getData(1).getLast() << endl;
      cout << "item2: " << temp.getData(1).getID() << endl;
      break;
    }
  }
}
// Add page from static data and vector
void CSPClient::addPage(string staticData, vector<Insertion>& dynamic) {
  pages.emplace_back(Page(staticData, dynamic));
}
// Add Page from file name:
void CSPClient::addPage(string staticData) {
  pages.emplace_back(Page(staticData));
}

void CSPClient::addPage(string staticFile, string offsetFile) {
  pages.emplace_back(Page(staticFile, offsetFile));
}

int main(int argc, char* argv[]) {
  CSPClient testClient("127.0.0.1", 8000);
  vector<Insertion> test;
  int pageNumber;
  cout << "Input page number: ";
  cin >> pageNumber;
  test.push_back(Insertion(10, UINT4));
  for (int i = 0; i < 7; i++) {
    testClient.addPage("test", test);
  }
  testClient.addPage("../data/benchmark0.txt");
  testClient.addPage("../data/benchmark1.txt");
  testClient.addPage("../data/benchmark2.txt");
  testClient.addPage("../data/benchmark3.txt");
  testClient.addPage("../data/benchmark4.txt");
  testClient.addPage("../data/benchmark5.txt");
  // Start clock for benchmarking:
  auto start = chrono::high_resolution_clock::now();
  // Run benchmarking loop (i = 100)
  // for (int i = 0; i < 5; i++) {
  pageBuffer testContent0 = testClient.pageRequest(pageNumber);
  //}
  // End clock
  auto stop = chrono::high_resolution_clock::now();
  // Print out time
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
  testContent0.printPage();
  cout << "\nTime in microseconds: " << duration.count() << '\n';
}