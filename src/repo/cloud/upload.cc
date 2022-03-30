// TODO: Check if downloaded file is diff from original - checksum
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <fstream>
// https://curl.se/libcurl/c/libcurl-tutorial.html
// #include <curl/curl.h>
// https://pragmaticjoe.blogspot.com/2015/09/uploading-file-with-google-drive-api.html
// https://stackoverflow.com/questions/41958236/posting-and-receiving-json-payload-with-curlpp#41974669

#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

#include "json.hpp"

std::string invoke(std::string url, std::string body) {
  std::list<std::string> header;
  header.push_back("Content-Type: application/json");

  curlpp::Cleanup clean;
  curlpp::Easy r;
  r.setOpt(new curlpp::options::Url(url));
  r.setOpt(new curlpp::options::HttpHeader(header));
  r.setOpt(new curlpp::options::PostFields(body));
  r.setOpt(new curlpp::options::PostFieldSize(body.length()));

  std::ostringstream response;
  r.setOpt(new curlpp::options::WriteStream(&response));

  r.perform();

  return std::string(response.str());
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <file>" << std::endl;
    return 1;
  }

  std::ifstream fi(argv[1]);
  if (!fi.is_open()) {
    std::cerr << "File not found: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  fi.seekg(0, std::ios::end);
  size_t size = fi.tellg();
  fi.seekg(0, std::ios::beg);
  std::vector<uint8_t> f(size);
  fi.read((char *)f.data(), size);
  fi.close();
  std::string f_str = std::string(f.begin(), f.end());

  // Getting tokens from environment.
  std::string client_id = getenv("GOOGLE_CLIENT_ID");
  std::string client_secret = getenv("GOOGLE_CLIENT_SECRET");
  std::string refresh_token = getenv("GOOGLE_REFRESH_TOKEN");

  // Get temp access token.
  curlpp::Easy req;
  // ACCESS_TOKEN=$(curl -d client_id="$GOOGLE_CLIENT_ID" \
        //     -d client_secret="$GOOGLE_CLIENT_SECRET" \
        //     -d refresh_token="$GOOGLE_REFRESH_TOKEN" \
        //     -d grant_type=refresh_token \
        //     https://accounts.google.com/o/oauth2/token | head -2 | cut -d'"' -f4 | tail -1)
  req.setOpt(
      new curlpp::options::Url("https://accounts.google.com/o/oauth2/token"));
  std::string fields =
      "client_id=" + client_id + "&client_secret=" + client_secret +
      "&refresh_token=" + refresh_token + "&grant_type=refresh_token";
  req.setOpt(new curlpp::options::PostFields(fields));
  req.setOpt(new curlpp::options::PostFieldSize(fields.length()));
  std::ostringstream response;
  req.setOpt(new curlpp::options::WriteStream(&response));
  req.perform();

  nlohmann::json j = nlohmann::json::parse(response.str());
  std::string access_token = j["access_token"].get<std::string>();

  // UPLOAD_ID=$(curl -X POST -L \
        //     -H "Authorization: Bearer $ACCESS_TOKEN" \
        //     -F "metadata={name :'$FILENAME'};type=application/json;charset=UTF-8" \
        //     -F "file=@$FILENAME;type=$(file --mime-type -b "$FILENAME")" \
        //     "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart" | head -3 | cut -d'"' -f4 | tail -1)
  // TODO: Need to add metadata for filename based on argv[1]. (needs to be its
  // own field?)
  curlpp::Easy req2;
  req2.setOpt(new curlpp::options::Url(
      "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart"));
  std::list<std::string> header;
  header.push_back("Authorization: Bearer " + access_token);
  req2.setOpt(new curlpp::options::HttpHeader(header));

  std::string fields2;
  fields2 += f_str;
  // std::string metadata = "{name :'" + std::string(basename(argv[1]))
  // + "';type=application/json;charset=UTF-8}";
  // std::string fields2 = "metadata=" + metadata + "&" + "file=@" +
  // std::string(argv[1]) + '&' + f_str;
  req2.setOpt(new curlpp::options::PostFields(fields2));

  std::ostringstream response2;
  req2.setOpt(new curlpp::options::WriteStream(&response2));
  req2.perform();

  nlohmann::json j2 = nlohmann::json::parse(response2.str());
  std::string upload_id = j2["id"].get<std::string>();

  std::cout << "File uploaded to:" << '\n'
            << "https://drive.google.com/file/d/" << upload_id << std::endl;

  return EXIT_SUCCESS;
}
