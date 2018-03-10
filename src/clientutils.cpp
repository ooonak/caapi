#include "clientutils.hpp"
#include <iostream>

bool ClientUtils::makeCsr(const x509Conf &config,
                          const std::string &keyFileName,
                          const std::string &csrFileName) {
  bool result = false;
  EVP_PKEY *pKey = nullptr;
  X509_REQ *pX509Req = nullptr;

  do {
    dbg("Generating RSA key");
    pKey = generateKey();
    if (!pKey)
      break;

    dbg("Saving RSA key");
    if (!saveKey(pKey, keyFileName))
      break;

    dbg("Generating certificate signing request (CSR)");
    pX509Req =
        generateCsr(pKey, config.commonName, config.email, config.country,
                    config.stateOrProvinceName, config.localityName,
                    config.organizationName, config.organizationalUnit);

    if (!pX509Req)
      break;

    dbg("Saving CSR");
    if (!saveCsr(pX509Req, csrFileName))
      break;

    result = true;
    dbg("Done!");
  } while (0);

  dbg("Cleanup...");
  if (pKey)
    EVP_PKEY_free(pKey);

  if (pX509Req)
    X509_REQ_free(pX509Req);

  return true;
}

/*
bool ClientUtils::verifyCert(const std::string &certFileName, const std::string
&caFileName) const { return false;
}
*/
