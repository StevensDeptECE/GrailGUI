#include "template.hh"

GrailRepository::GrailRepository(uint32_t size) : regions(64) {
  std::vector<uint8_t> bytes(size, 0);
}

GrailRepository::~GrailRepository() {}

void GrailRepository::generateKey(const std::string& siteName) {
  int bits = 2048;

  BIGNUM* bne = BN_new();
  int ret = BN_set_word(bne, RSA_F4);
  if (ret != 1) std::cerr << "Error: Creating bignum failed." << std::endl;

  RSA* r = RSA_new();
  ret = RSA_generate_key_ex(r, bits, bne, NULL);
  if (ret != 1) std::cerr << "Error: Generating key pair failed." << std::endl;

  // // XXX: Should we save keys to disk?
  // BIO *bp_public = BIO_new_file("public.pem", "w+");
  // ret = PEM_write_bio_RSAPublicKey(bp_public, r);
  // if (ret != 1)
  //     std::cerr << "Error: Writing public key failed." << std::endl;

  // BIO *bp_private = BIO_new_file("private.pem", "w+");
  // ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL,
  // NULL); if (ret != 1)
  //     std::cerr << "Error: Writing private key failed." << std::endl;

  // Couple each site with its keypair.
  siteKeys.push_back(std::make_pair(siteName, r));
}

void GrailRepository::deleteKey(const std::string& siteName) {
  for (int i = 0; i < siteKeys.size(); ++i)
    if (siteKeys[i].first == siteName) {
      siteKeys.erase(siteKeys.begin() + i);
      return;
    }

  std::cerr << "Error: Site: " << siteName << " not in site list." << std::endl;
}

void GrailRepository::backupToCloud(const std::string& service_name,
                                    const std::string& userid,
                                    const std::string& passwd, uint32_t offset,
                                    uint32_t stride, std::string& img_name) {
  try {
    SteganographicImage steg(img_name, offset, stride);

    steg.hide_secret(bytes);
    steg.write_webp("new" + img_name);

  } catch (char const* e) {
    std::cerr << "Error: " << e << std::endl;
  }
};

void GrailRepository::restoreFromCloud(const std::string& service_name,
                                       const std::string& userid,
                                       const std::string& passwd,
                                       uint32_t offset, uint32_t stride,
                                       std::string& img_name) {
  try {
    SteganographicImage steg(img_name, offset, stride);

    bytes = steg.recover();

    std::string base64 = base64_encode(bytes.data(), bytes.size());
    std::cout << base64 << std::endl;

  } catch (char const* e) {
    std::cerr << "Error: " << e << std::endl;
  }
}

void unlock(const std::string& password, const Factor2& factor,
            const std::string& area){};

void GrailRepository::scramble(const std::string& area) {
  std::independent_bits_engine<std::default_random_engine, CHAR_BIT, uint8_t>
      rbe;
  std::generate(bytes.begin(), bytes.end(), std::ref(rbe));
}

void GrailRepository::destroy() { std::fill(bytes.begin(), bytes.end(), 0); }

// PubKey getPublicKey(const std::string& password, const Factor2& factor, const
// std::string& area)
// }

uint64_t GrailRepository::proveIdentity(const std::string& password,
                                        uint64_t nonce,
                                        const std::string& area) {
  return 0;
}

void GrailRepository::getUserid(const std::string& area,
                                const std::string& name, std::string& userid,
                                std::string& passwd){};

// TODO: Require hardware authentication (à la YubiKey)?
void GrailRepository::recover2ndFactor(){};
