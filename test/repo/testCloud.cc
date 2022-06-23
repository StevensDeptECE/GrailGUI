#include "repo/cloud/Cloud.hh"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <upload|download> <file>" << '\n';
    return 1;
  }
  CloudClient client;
  if (!strcmp(argv[1], "upload"))
    client.upload(argv[2]);
  else if (!strcmp(argv[1], "download")) {
    if (argc != 4) {
      std::cout << "Usage: " << argv[0] << " download <file_name> <file_id>"
                << '\n';
      return EXIT_FAILURE;
    }
    client.download(argv[2], argv[3]);
  } else {
    std::cout << "Usage: " << argv[0] << " <upload|download> <file>" << '\n';
  }
  return EXIT_SUCCESS;
}
