#include <fstream>
#include <string>

#include "opengl/Errcode.hh"
#include "util/Ex.hh"

// #include <gio/gio.h>
#include <openssl/sha.h>

#include <curlpp/Easy.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include "json.hpp"
using json = nlohmann::json;

class CloudClient {
 private:
  // std::string file_name;
  // std::string file_id;
  // std::string file_url;
  std::string file_bytes;
  std::string file_hash;
  std::string service = "google";

  curlpp::Easy req;
  std::ostringstream response;
  std::string access_token;
  std::string client_id, client_secret, refresh_token;

  void get_file_bytes(const std::string& fname);
  std::string to_hex(unsigned char s);
  std::string sha256(const std::string& str);

 public:
  CloudClient();
  ~CloudClient();

  void get_access_token();
  void upload(const std::string& file_name);
  void download(const std::string& file_name, const std::string& file_id);
  void delete_file(const std::string& file_name);
  void list_files();
  std::string invoke(const std::string& url, const std::string& body);
};
