/*
** Decompress a stream into RAM directly, to be used to loaompressed
*BlockLoaders
*/
#include <errno.h>

#include <cstdlib>
#include <fstream>
#include <list>
#include <memory>
#include <string>

#include "xp/lzmautil.hh"

using namespace std;
/*
** Initialize the decoder and allocate the maximum 65Mb RAM to handle any
*incoming stream
*/
class Decompressor {
 private:
  constexpr static size_t BUFSIZE = 1 << 16;
  lzma_stream strm;
  uint8_t inbuf[BUFSIZE];

  unique_ptr<uint8_t[]> decomp_mem;
  uint64_t cur_ind;
  void check_resize(size_t write_size) {
    uint8_t* cur_arr = decomp_mem.get();
    if (cur_ind + write_size >= sizeof(cur_arr) / sizeof(*cur_arr)) {
      uint8_t* ptr_new = (uint8_t*)realloc(cur_arr, sizeof(cur_arr) * 2);
      if (!ptr_new) {
        // throw memory error with Ex1
      }

      decomp_mem.release();
      decomp_mem.reset(ptr_new);
    }
  }

 public:
  Decompressor() : decomp_mem(make_unique<uint8_t[]>(BUFSIZE)) {
    // TODO: perhaps load non-concatenated?
    strm = LZMA_STREAM_INIT;

    lzma_ret ret = lzma_stream_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);
    if (ret != LZMA_OK) throw ret;  // TODO: throw Ex1(  );
    // Errs: LZMA_MEM_ERROR: Mem alloc, LZMA_OPTIONS_ERROR: bad flags
    // default, probably LZMA_PROG_ERROR
  }

  ~Decompressor() { lzma_end(&strm); }

  /*
  ** Load one file and decompress it into a BlockLoader?
  */
  void decompressFileToRAM(const char compressedFile[]) {
    lzma_action action = LZMA_RUN;
    ifstream in(compressedFile, ios::binary);

    if (!in.good()) {
      throw Ex1(Errcode::FILE_NOT_FOUND);
    }

    uint8_t inbuf[BUFSIZE];
    uint8_t outbuf[BUFSIZE];

    // call lzma stream decoder again?
    strm.next_in = nullptr;
    strm.avail_in = 0;
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);

    while (true) {
      if (strm.avail_in == 0 && in.peek() != EOF) {
        strm.next_in = inbuf;
        strm.avail_in = in.read((char*)inbuf, BUFSIZE).gcount();
      }
      lzma_ret ret = lzma_stream_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);
      if (ret != LZMA_OK) throw ret;  // TODO: throw Ex1(  );

      ret = lzma_code(&strm, action);

      if (ret != LZMA_OK) throw ret;  // TODO: throw Ex1(  );
      if (strm.avail_out == 0 || ret == LZMA_STREAM_END) {
        // Resize pointer if needed
        size_t write_size = sizeof(outbuf) - strm.avail_out;
        check_resize(write_size);

        // Write to buffer
        copy(outbuf, outbuf + write_size, decomp_mem.get());
        cur_ind += write_size;
        strm.next_out = outbuf;
        strm.avail_out = sizeof(outbuf);
      }

      lzma_err(ret);
    }
    // The uncompressed size is shown when running xz --list <file>. By taking a
    // closer look, this is calculated by running the
    // lzma_index_uncompressed_size() function on a lzma_quite index populated
    // by a parse big_indexes() function from the xz command line tool.
  }
  void decompressFile(const char compressedFile[], const char outFile[]) {
    lzma_action action = LZMA_RUN;
    ifstream in(compressedFile, ios::binary);
    ofstream out(outFile, ios::binary);

    if (!in.good()) {
      throw Ex1(Errcode::FILE_NOT_FOUND);
    }

    uint8_t inbuf[BUFSIZE];
    uint8_t outbuf[BUFSIZE];

    // call lzma stream decoder again?
    strm.next_in = nullptr;
    strm.avail_in = 0;
    strm.next_out = outbuf;
    strm.avail_out = sizeof(outbuf);

    while (true) {
      if (strm.avail_in == 0 && in.peek() != EOF) {
        strm.next_in = inbuf;
        strm.avail_in = in.read((char*)inbuf, BUFSIZE).gcount();
      }

      if (in.peek() == EOF) {
        action = LZMA_FINISH;
      }

      lzma_ret ret = lzma_stream_decoder(&strm, UINT64_MAX, LZMA_CONCATENATED);
      if (ret != LZMA_OK) throw ret;  // TODO: throw Ex1(  );

      ret = lzma_code(&strm, action);

      if (strm.avail_out == 0 || ret == LZMA_STREAM_END) {
        size_t write_size = sizeof(outbuf) - strm.avail_out;
        out.write((char*)outbuf, write_size);
        strm.next_out = outbuf;
        strm.avail_out = sizeof(outbuf);
        return;
      }

      lzma_err(ret);
    }
  }
};

int main(int argc, char** argv) {
  if (argc <= 1) {
    fprintf(stderr, "Usage: %s FILES...\n", argv[0]);
    return EXIT_FAILURE;
  }

  Decompressor decomp;
  // Try to decompress all files.
  for (int i = 1; i < argc; ++i) {
    string argv_mod = argv[i];
    int ind = argv_mod.find_last_of('.');
    argv_mod.resize(ind);

    decomp.decompressFile(argv[i], argv_mod.c_str());
  }
}
