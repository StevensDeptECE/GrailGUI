#include "BlockMapLoader.hh"

void BlockLoader::init(uint64_t bytes, Type t, uint32_t version)
	: mem(std::make_unique<uint64_t>
				(new uint64_t[getHeaderSize() + (bytes + 7) / 8])) {
	generalHeader = (GeneralHeader*)mem;                                    // header is the first chunk of bytes
	generalHeader->magic = ((((('!' << 8) + 'B') << 8) + 'L') << 8) + 'd';  // magic number for all block loaders
	generalHeader->type = uint32_t(t);
	generalHeader->version = version;
	securityHeader = (SecurityHeader*)((uint64_t*)mem + sizeof(GeneralHeader) / 8);
}

void BlockMapLoader::save(const char filename[]) {
	int fh = open(filename, O_WRONLY);
	write(fh, (char*)mem, size);
	close(fh);
	// byte-endian-ness matters! You cannot write this in on an Intel and read in on Sparc
	
	// if you have to worry about endianness
	// float: b1 b2 b3 b4 uint32_t:   b1 b2 b3 b4   b4 b3 b2 b1 b4 b3 b2 b1
	// uint64_t:   b1 b2 b3 b4 b5 b6 b7 b8 --> b8 b7 b6 b5 b4 b3 b2 b1
}
BlockMapLoader::BlockMapLoader(const char filename[]) {
	int fh = open(filename, O_RDONLY);
	st_stat s;
	stat(fh, &s);
	mem = new uint64_t[(s.st_size +7)/ 8];
	read(fh, (char*)mem, size);
	close(fh);
	init();
	// floats are now completely loaded, ready to draw!
}

Method BlockMapLoader::methods[] = {
  
};

/*
	TODO: For now we will just call methodPolygon
	Later we have to call the method that applies to the type.
	We will group all of the same type together so they can be
	executed in a single block.

	Perhaps teh methods should contain a start index and length to draw
 */
void BlockMapLoader::methodPolygon() {
	
}

void BlockMapLoader::methodPolyline() {

}

