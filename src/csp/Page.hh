#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include "util/datatype.hh"
#include "csp/Insertion.hh"
#include <fstream>

using namespace std;

class Page {

private:
	string staticContent; //static content
	std::vector<Insertion> insertions; //dynamic content
	//Make field for full size, update each time dynamic content is added (static + dynamic)
	uint32_t size;
	uint32_t staticSize;
	void readOffsetVector(ifstream& infile);

public:
    //Constructors
    Page() {}
	Page(const string& inputFile) ;
	Page(const string& staticContent, vector<Insertion> insertions) : staticContent(staticContent), insertions(insertions), size(staticContent.size()) {
	    for (auto i : insertions)
	        size += dynamicSize(i.d);
	}
	Page(const string& staticFile, const string& insertFile);
    //Getters
	char* getStaticPointer() {return &staticContent[0u];}
	string getStaticContent() const {return staticContent;}
	vector<Insertion> getInsertions() const {return insertions;}
	uint32_t getSize() const {return size;}
	uint32_t getStaticSize() const {return staticSize;}

	//Methods
	void addDynamic(uint32_t offset, DataType d) {
		insertions.emplace_back(Insertion(offset, d));
		size += dynamicSize(d);
	}

	void addStatic(const string& t) {
		staticContent += t;
	}

	//TODO: make function to add static content
	bool isStatic() {return insertions.empty();}
	uint32_t dynamicSize(DataType d);

	friend ostream& operator <<(ostream& s, const Page& p);
};

//TODO: Write function to write file