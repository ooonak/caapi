#ifndef __CERTUTILS_HPP__
#define __CERTUTILS_HPP__

#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/x509v3.h>
#include <string>

#include "dbg.hpp"

#define DEFAULT_RSA_KEYLENGTH 4096               // 2048
#define DEFAULT_SIGNATURE_ALGORITHM EVP_sha512() // EVP_sha256()

class CertUtils {
public:
  explicit CertUtils();

protected:
  /* ===== Client methods ===== */

  /**
   * @brief generateKey
   * @param keyLength
   * @return
   */
  EVP_PKEY *generateKey(int keyLength = DEFAULT_RSA_KEYLENGTH) const;

  /**
   * @brief generateCsr
   * @param pKey
   * @param commonName
   * @param email
   * @param country
   * @param stateOrProvinceName
   * @param localityName
   * @param organizationName
   * @param organizationalUnit
   * @param signatureAlgorithm
   * @return
   */
  X509_REQ *generateCsr(
      EVP_PKEY *pKey, const std::string &commonName, const std::string &email,
      const std::string &country = std::string(""),
      const std::string &stateOrProvinceName = std::string(""),
      const std::string &localityName = std::string(""),
      const std::string &organizationName = std::string(""),
      const std::string &organizationalUnit = std::string(""),
      const EVP_MD *signatureAlgorithm = DEFAULT_SIGNATURE_ALGORITHM) const;

  /**
   * @brief saveKey
   * @param pKey
   * @param keyName
   * @return
   */
  bool saveKey(EVP_PKEY *pKey, const std::string &keyName) const;

  /**
   * @brief saveCsr
   * @param pX509Req
   * @param csrName
   * @return
   */
  bool saveCsr(X509_REQ *pX509Req, const std::string &csrName) const;

  /* ===== CA methods ===== */
  bool loadCaCertAndKey(const std::string &certFile, X509 **cert, const std::string &keyFile, EVP_PKEY **key);

private:
};

#endif // __CERTUTILS_HPP__
