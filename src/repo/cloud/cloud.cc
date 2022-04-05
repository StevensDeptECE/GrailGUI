#include <fstream>
#include <string>
// https://curl.se/libcurl/c/libcurl-tutorial.html
// #include <curl/curl.h>
// https://pragmaticjoe.blogspot.com/2015/09/uploading-file-with-google-drive-api.html

// #include <gio/gio.h>
#include <openssl/sha.h>

#include <curlpp/Easy.hpp>
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

  void get_file_bytes(std::string fname) {
    std::ifstream fi(fname);
    if (!fi.is_open()) throw "File not found: " + fname;
    // std::string str((std::istreambuf_iterator<char>(fi)),
    //                 std::istreambuf_iterator<char>());
    fi.seekg(0, std::ios::end);
    size_t size = fi.tellg();
    fi.seekg(0, std::ios::beg);
    std::vector<uint8_t> f(size);
    fi.read((char *)f.data(), size);
    fi.close();
    file_bytes = std::string(f.begin(), f.end());
  }

  std::string to_hex(unsigned char s) {
    std::stringstream ss;
    ss << std::hex << (int)s;
    return ss.str();
  }

  std::string sha256(std::string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string output = "";
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) output += to_hex(hash[i]);
    std::cout << "SHA256: " << output << std::endl;
    return output;
  }

 public:
  CloudClient() {
    // Getting tokens from environment.
    client_id = getenv("GOOGLE_CLIENT_ID"),
    client_secret = getenv("GOOGLE_CLIENT_SECRET"),
    refresh_token = getenv("GOOGLE_REFRESH_TOKEN");
  };
  ~CloudClient(){};

  void get_access_token() {
    // Get temp access token.
    req.setOpt(
        new curlpp::options::Url("https://accounts.google.com/o/oauth2/token"));
    std::string fields =
        "client_id=" + client_id + "&client_secret=" + client_secret +
        "&refresh_token=" + refresh_token + "&grant_type=refresh_token";
    req.setOpt(new curlpp::options::PostFields(fields));
    req.setOpt(new curlpp::options::PostFieldSize(fields.length()));

    response.str("");
    req.setOpt(new curlpp::options::WriteStream(&response));
    req.perform();

    access_token =
        json::parse(response.str())["access_token"].get<std::string>();
  }

  // XXX: There does not appear to be a way to access Google Drive's download
  // functionality from anything other than their Drive API for Java, Python,
  // or Node.js
  void upload(std::string file_name) {
    get_file_bytes(file_name);
    // Write file hash to file.
    std::ofstream fo(file_name + ".sha256");
    fo << sha256(file_bytes);
    fo.close();
    get_access_token();

    if (service == "google") {
      req.setOpt(
          new curlpp::options::Url("https://www.googleapis.com/upload/drive/v3/"
                                   "files?uploadType=multipart"));
      std::list<std::string> header;
      header.push_back("Authorization: Bearer " + access_token);
      req.setOpt(new curlpp::options::HttpHeader(header));

      curlpp::Forms formParts;
      // Push metadata to form with filetype
      formParts.push_back(new curlpp::FormParts::Content(
          "metadata", "{name :'" + file_name + "'}", "application/json"));
      // TODO: Might want to try getting MIME type of given file?
      formParts.push_back(new curlpp::FormParts::File("file", file_name));
      req.setOpt(new curlpp::options::HttpPost(formParts));

      // Reset response
      response.str("");
      req.perform();
      req.setOpt(new curlpp::options::WriteStream(&response));

      std::string upload_id =
          json::parse(response.str())["id"].get<std::string>();

      std::cout << "File uploaded to:" << '\n'
                << "https://drive.google.com/file/d/" << upload_id << std::endl;
    }
  }

  // TODO: Check if downloaded file is diff from original - checksum
  void download(std::string file_name) {
    get_access_token();
    // Download from Google Drive.
    get_file_bytes(file_name);

    // Compare hash of downloaded file to original.
    std::string downloaded_f_hash = file_bytes;
    std::cout << "Downloaded file hash: " << downloaded_f_hash << std::endl;
  }

  void delete_file(std::string file_name);
  void list_files();

  // https://stackoverflow.com/questions/41958236/posting-and-receiving-json-payload-with-curlpp#41974669
  std::string invoke(std::string url, std::string body) {
    std::list<std::string> header;
    header.push_back("Content-Type: application/json");

    curlpp::Cleanup clean;
    curlpp::Easy r;
    req.setOpt(new curlpp::options::Url(url));
    req.setOpt(new curlpp::options::HttpHeader(header));
    req.setOpt(new curlpp::options::PostFields(body));
    req.setOpt(new curlpp::options::PostFieldSize(body.length()));

    std::ostringstream response;
    req.setOpt(new curlpp::options::WriteStream(&response));

    req.perform();

    return std::string(response.str());
  }
};

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }
  try {
    CloudClient client;
    client.upload(argv[1]);
  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
