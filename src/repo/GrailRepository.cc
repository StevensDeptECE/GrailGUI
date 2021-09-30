#include <openssl/ssl.h>
#include <iostream>
#include <vector>

std::vector<std::pair<std::string, RSA *>> siteKeys;

void generateKey(const std::string& siteName)
{
    int ret = 0;
    RSA *r = NULL;
    BIGNUM *bne = NULL;
    BIO *bp_public = NULL, *bp_private = NULL;
    int bits = 2048;
	unsigned long e = RSA_F4;

    bne = BN_new();
	ret = BN_set_word(bne, e);
	if (ret != 1)
        std::cerr << "Error: Creating bignum failed." << std::endl;

	r = RSA_new();
	ret = RSA_generate_key_ex(r, bits, bne, NULL);
	if (ret != 1)
        std::cerr << "Error: Generating key pair failed." << std::endl;

    // XXX: Should we save keys to disk?
	bp_public = BIO_new_file("public.pem", "w+");
	ret = PEM_write_bio_RSAPublicKey(bp_public, r);
	if (ret != 1)
        std::cerr << "Error: Writing public key failed." << std::endl;

	bp_private = BIO_new_file("private.pem", "w+");
	ret = PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL);
	if (ret != 1)
        std::cerr << "Error: Writing private key failed." << std::endl;

    // Couple each site with its keypair.
    siteKeys.push_back(std::make_pair(siteName, r));
}

void deleteKey(const std::string& siteName)
{
    for (int i = 0; i < siteKeys.size(); ++i)
        if (siteKeys[i].first == siteName) {
            siteKeys.erase(siteKeys.begin() + i);
            return;
        }

    std::cerr << "Error: Site: " << siteName << " not in site list."
        << std::endl;
}

void backupToCloud(const std::string& service_name, const std::string& userid,
        const std::string& passwd, uint32_t offset, uint32_t stride,
        uint32_t bits);

void restoryFromCloud(const std::string& service_name,
        const std::string& userid, const std::string& passwd, uint32_t offset,
        uint32_t stride, uint32_t bits);

void unlock(const std::string& password, const Factor2& factor,
        const std::string& area); 

void scramble(const std::string& area);
  
void destroy();
  
PubKey getPublicKey(const std::string& password, const Factor2& factor,
        const std::string& area);
  
  
KeyPair getPublicKey(const std::string& password, const Factor2& factor,
        const std::string& area);

uint64_t proveIdentity(const std::string& password, uint64_t nonce,
        const std::string& area);
  
void getUserid(const std::string& area, const std::string& name,
        std::string& userid, std::string& passwd);
  
// TODO: Require hardware authentication (à la YubiKey)?
void recover2ndFactor();
