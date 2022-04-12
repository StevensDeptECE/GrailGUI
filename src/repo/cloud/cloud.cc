#include "cloud.hh"

void CloudClient::get_file_bytes(std::string fname) {
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

std::string CloudClient::to_hex(unsigned char s) {
  std::stringstream ss;
  ss << std::hex << (int)s;
  return ss.str();
}

std::string CloudClient::sha256(std::string str) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, str.c_str(), str.size());
  SHA256_Final(hash, &sha256);
  std::string output = "";
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) output += to_hex(hash[i]);
  // std::cout << "SHA256: " << output << std::endl;
  return output;
}

CloudClient::CloudClient() {
  // Getting tokens from environment.
  if (service == "google") {
    client_id = getenv("GOOGLE_CLIENT_ID"),
    client_secret = getenv("GOOGLE_CLIENT_SECRET"),
    refresh_token = getenv("GOOGLE_REFRESH_TOKEN");
  }
};
CloudClient::~CloudClient(){};

void CloudClient::get_access_token() {
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

  access_token = json::parse(response.str())["access_token"].get<std::string>();
}

void CloudClient::upload(std::string file_name) {
  get_file_bytes(file_name);
  // Write file hash to file.
  std::ofstream fo(file_name + ".sha256");
  fo << sha256(file_bytes);
  fo.close();
  if (service == "google") {
    get_access_token();
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

// XXX: There does not appear to be a way to access Google Drive's private
// download functionality from anything other than their Drive API for Java,
// Python, or Node.js
// NOTE: This only works with publicly-shared files
// https://stackoverflow.com/questions/25010369/wget-curl-large-file-from-google-drive
void CloudClient::download(std::string file_name, std::string file_id) {
  if (service == "google") {
    // get_access_token();
    // std::list<std::string> header;
    // header.push_back("Authorization: Bearer " + access_token);
    // req.setOpt(new curlpp::options::HttpHeader(header));

    // TODO: Need to save cookie from first GET request and use in second
    // request
    std::list<std::string> cookies;

    req.setOpt(
        new curlpp::options::Url("https://drive.google.com/"
                                 "uc?export=download&id=" +
                                 file_id));
    req.perform();

    curlpp::infos::CookieList::get(req, cookies);
    // for (auto &cookie : cookies) std::cout << cookie << std::endl;

    req.setOpt(
        new curlpp::options::Url("https://drive.google.com/"
                                 "uc?export=download&confirm=&id=" +
                                 file_id));
    response.str("");
    req.setOpt(new curlpp::options::WriteStream(&response));

    std::ofstream out_file(file_name);
    for (char c : response.str()) out_file << c;
  }

  get_file_bytes(file_name);
  // std::cout << file_bytes << std::endl;

  // Compare hash of downloaded file to original.
  std::ifstream fi(file_name + ".sha256");
  if (!fi.is_open()) throw "File not found: " + file_name + ".sha256";
  std::string original_f_hash(std::istreambuf_iterator<char>(fi), {});
  fi.close();

  if (sha256(file_bytes) != original_f_hash)
    throw "Downloaded file is NOT THE SAME as the original.";
  else
    std::cout << "All good! Downloaded file is the same as the original."
              << std::endl;
}

// https://stackoverflow.com/questions/41958236/posting-and-receiving-json-payload-with-curlpp#41974669
std::string CloudClient::invoke(std::string url, std::string body) {
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

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage: " << argv[0] << " <upload|download> <file>"
              << std::endl;
    return 1;
  }
  try {
    CloudClient client;
    if (!strcmp(argv[1], "upload"))
      client.upload(argv[2]);
    else if (!strcmp(argv[1], "download")) {
      if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " download <file_name> <file_id>"
                  << std::endl;
        return EXIT_FAILURE;
      }
      client.download(argv[2], argv[3]);
    } else {
      std::cout << "Usage: " << argv[0] << " <upload|download> <file>"
                << std::endl;
    }
  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
