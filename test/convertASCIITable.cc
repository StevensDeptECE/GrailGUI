#include <iostream>

#include "QuoteTable.hh"

int main() {
	Table::loadASCII("res/aapl.txt", "res/aapl.bin");
	//	Table::loadASCII("res/ibm.quotes", "res/ibm.bin");
}
