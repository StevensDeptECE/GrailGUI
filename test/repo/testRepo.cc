#include "repo/Repository.hh"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <add|upload|download>" << std::endl;
    return EXIT_FAILURE;
  }

  GrailRepository repo(1024);
  std::string img_name = "test.webp";

  if (!strcmp(argv[1], "add")) {
    repo.generateKey("main", "test.com");
  } else if (!strcmp(argv[1], "upload")) {
    repo.backupToCloud("google", "userid", "password", 0, 1, img_name);
  } else if (!strcmp(argv[1], "download")) {
    repo.restoreFromCloud("google", "userid", "password", 0, 1, img_name);
  } else {
    std::cerr << "Usage: " << argv[0] << " <add|upload|download>" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
