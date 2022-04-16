#include <openssl/ssl.h>

#include <random>
#include <unordered_map>
#include <vector>

#include "../util/HashMap.hh"
#include "cloud/cloud.hh"
#include "stegano/steganography.hh"

class PubKey {};
class SymmetricKey {};
class Factor2 {};
class KeyPair {};
class GrailRepository {
 private:
  SymmetricKey key;  // 2nd key to unlock this file
  // note: this example implementation shows only how to create one file,not the
  // alternate versions in feature 4
  CloudClient client;

  union {
    // A stack storing: region_name -> (site_name -> key)
    std::vector<std::pair<std::string, std::unordered_map<std::string, RSA*>>>
        siteKeys;
    std::vector<uint8_t> bytes;  // the raw bytes of the repository
  };

 public:
  GrailRepository(
      uint32_t size);  // set up empty repo with size bytes of storage
  ~GrailRepository();
  GrailRepository(const GrailRepository& orig) = delete;
  GrailRepository& operator=(const GrailRepository& orig) = delete;
  // generate a new key for the specified website
  void generateKey(const std::string& regionName, const std::string& siteName);
  void deleteKey(const std::string& siteName);  // unregister for site?

  void backupToCloud(const std::string& service_name, const std::string& userid,
                     const std::string& passwd, uint32_t offset,
                     uint32_t stride, std::string& img_name);

  void restoreFromCloud(const std::string& service_name,
                        const std::string& userid, const std::string& passwd,
                        uint32_t offset, uint32_t stride,
                        std::string& img_name);

  // using a password and possible 2nd factor authentication, unlock the named
  // area decrypting a range of bytes
  void unlock(const std::string& password, const Factor2& factor,
              const std::string& area);

  // Overwrite the array in RAM with random numbers
  void scramble(const std::string& area);

  // Scramble array with random values and write back to disk.
  void destroy();

  // extract public key
  PubKey getPublicKey(const std::string& password, const Factor2& factor,
                      const std::string& area);
  // extract key pair so it can be used
  KeyPair getKeyPair(const std::string& password, const Factor2& factor,
                     const std::string& area);

  // Given digital authentication challenge, take the given nonce, decrypt it,
  // and send it back. note that this could perhaps be used in an attack, so
  // perhaps rather than giving anyone an answer to their challenge, send the
  // answer back encrypted so that only the site claiming to be sending it can
  uint64_t proveIdentity(const std::string& password, uint64_t nonce,
                         const std::string& area);

  // in area “main” get website “gmail.com” get userid and password
  void get(const std::string& area, const std::string& name,
           std::string& userid, std::string& passwd);

  // The hard, research optional area. How do we recover if a 2nd factor
  // authentication is lost? how do we prevent attackers from erroneously
  // claiming 2nd factor is lost to bypass it?
  void recover2ndFactor();
};
