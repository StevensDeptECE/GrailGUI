#include "csp/csp.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// our application errors must be defined before Ex.hh
#include "csp/IPV4Socket.hh"
#include "CSPRequest.hh"
#include <cstdlib>
#include "cspservlet/UtilityStructs.hh"
#include "util/List1.hh"
using namespace std;

Log srvlog; // log all important events for security and debugging

int main(int argc, char* argv[]) {
	try {
		IPV4Socket s("127.0.0.1",8000);
		int ActualTestNumber=2;
		int testNumber = ActualTestNumber-1;
		s.send(testNumber);
		Buffer& in = s.getIn();
		switch(testNumber) {
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
			case 2:{
				int l1=(int)in.read<uint8_t >();
				for(int i=0;i<l1;i++){
					cout<<in.read<char>();
				}
				cout<<endl;
				l1=(int)in.read<uint8_t >();
				for(int i=0;i<l1;i++){
					cout<<in.read<char>();
				}
				cout<<endl;
				//TODO: Need a way to know when @ end of buffer
				break;
			}case 3:{
				set4 temp = in.read<set4>();
				dump4(temp);
				break;
			}
			case 4: {
				List1<uint32_t> temp=in.readList<uint32_t>(); // <List1<uint32_t>>();
				//cout<<"Used: "<<temp.getUsed()<<endl;
				//cout<<"Capacity: "<<temp.getCapacity()<<endl;
				//cout<<"Slot 0: "<<temp[0]<<endl;
				//in.read<uint32_t>();
				for(uint i=0;i<15;i++){
					//cout<<"Expected: "<<(int)in.read<uint32_t>()<<"\t\tActual: "<<(int)temp[i]<<endl;
					//cout<<in.read<uint32_t>()<<endl;

					//cout<<temp[i]<<endl;
					cout<<temp.getData(i)<<endl;
				}
				//cout<<sizeof(in)<<endl;

				break;
			}case 5: {
				//int l1=(int)in.read<uint32_t >();
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
				cout<<temp.getFirst()<<endl;
				cout<<temp.getLast()<<endl;
				cout<<(int)temp.getID()<<endl;
				break;
			}case 6:{
				//List1<Student> temp=in.readList<Student>();
				//Student item1 = temp.getData(0);
				//cout<<temp.getUsed()<<endl;
				//cout<<"empty"<<endl;
				List1<Student> temp=in.readStudentList();
				//Student item1 = temp.getData(0);
				//cout<<temp.getUsed()<<endl;
				//cout<<"item1: "<<item1.getFirst()<<endl;
                cout<<"item1: "<<temp.getData(0).getFirst()<<endl;
				cout<<"item1: "<<temp.getData(0).getLast()<<endl;
				cout<<"item1: "<<temp.getData(0).getID()<<endl;
                cout<<endl;
                cout<<"item2: "<<temp.getData(1).getFirst()<<endl;
                cout<<"item2: "<<temp.getData(1).getLast()<<endl;
                cout<<"item2: "<<temp.getData(1).getID()<<endl;
				break;
			}
		}

	} catch (const Ex& e) {
		cerr << e << '\n';
	}
	return 0;
}
