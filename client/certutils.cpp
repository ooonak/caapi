﻿#include "certutils.hpp"
#include <iostream>
#include <unistd.h>
#include <openssl/opensslv.h>
#include <openssl/rand.h>

CertUtils::CertUtils() {
    // TODO OPENSSL_VERSION_AT_LEAST(1, 1);
    // OpenSSL_version_num()

    std::cout << "Developed against OpenSSL 1.1.0g" << std::endl;
    std::cout << "Your version is: " << OpenSSL_version(OPENSSL_VERSION) << std::endl;
}

EVP_PKEY *CertUtils::generateKey(int keyLength) const {
  bool result = false;
  RSA *rsaKeyPair = nullptr;
  BIGNUM *bigNumber = nullptr;
  EVP_PKEY *pKey = nullptr;

  // Sneaky way to handle cleanup on failure nicely
  do {
    // Generate RSA key pair object
    rsaKeyPair = RSA_new();
    if (!rsaKeyPair) {
      err("Could not generate RSA key (RSA_new())");
      break;
    }

    // We need a big number object
    bigNumber = BN_new();
    if (!bigNumber) {
      err("Could not generate big number (BN_new())");
      break;
    }

    if (!BN_set_word(bigNumber, 65537)) {
      err("Could not set word for big number (BN_set_word())");
      break;
    }

    // Actually generate RSA key pair
    if (!RSA_generate_key_ex(rsaKeyPair, keyLength, bigNumber, nullptr)) {
      err("Could not generate RSA key (RSA_generate_key_ex())");
      break;
    }

    // Generate private key object
    pKey = EVP_PKEY_new();
    if (!pKey) {
      err("Unable to create EVP_PKEY structure (EVP_PKEY_new())");
      break;
    }

    // Assign the key pair to the private key object
    if (!EVP_PKEY_assign_RSA(pKey, rsaKeyPair)) {
      err("Failed to assign keypair to key object (EVP_PKEY_assign_RSA())");
      break;
    }

    // rsaKeyPair is now handed over to pKey
    rsaKeyPair = nullptr;

    // Success
    result = true;
  } while (false);

  // Cleanup, error or not
  if (bigNumber)
    BN_free(bigNumber);

  if (!result) {
    if (pKey)
      EVP_PKEY_free(pKey);
    if (rsaKeyPair)
      RSA_free(rsaKeyPair);

    return nullptr;
  }

  return pKey;
}

X509_REQ *CertUtils::generateCsr(EVP_PKEY *pKey, const std::string &commonName,
                                 const std::string &email,
                                 const std::string &country,
                                 const std::string &stateOrProvinceName,
                                 const std::string &localityName,
                                 const std::string &organizationName,
                                 const std::string &organizationalUnit,
                                 const EVP_MD *signatureAlgorithm) const {
  if (!pKey) {
    err("Don't you dare throw a null pointer at me, you shall not parse!");
    return nullptr;
  }

  if (commonName.length() == 0 && email.length() == 0) {
    err("Both Common Name and email is an empty string, that would never work");
    return nullptr;
  }

  bool result = false;
  X509_REQ *pX509Req = nullptr;

  do {
    pX509Req = X509_REQ_new();
    if (!pX509Req) {
      err("Could not create X509 object (X509_REQ_new())");
      break;
    }

    // Set version 2, and get version 3
    X509_REQ_set_version(pX509Req, 2);

    X509_NAME *name = X509_REQ_get_subject_name(pX509Req);
    if (!name) {
      err("Could not get subject name (X509_REQ_get_subject_name())");
      break;
    }

    if (country.length() > 0)
      if (X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC,
                                     (unsigned char *)country.c_str(), -1, -1,
                                     0) != 1)
        break;
    if (stateOrProvinceName.length() > 0)
      if (X509_NAME_add_entry_by_txt(
              name, "ST", MBSTRING_ASC,
              (unsigned char *)stateOrProvinceName.c_str(), -1, -1, 0) != 1)
        break;
    if (localityName.length() > 0)
      if (X509_NAME_add_entry_by_txt(name, "L", MBSTRING_ASC,
                                     (unsigned char *)localityName.c_str(), -1,
                                     -1, 0) != 1)
        break;
    if (organizationName.length() > 0)
      if (X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC,
                                     (unsigned char *)organizationName.c_str(),
                                     -1, -1, 0) != 1)
        break;
    if (organizationalUnit.length() > 0)
      if (X509_NAME_add_entry_by_txt(
              name, "OU", MBSTRING_ASC,
              (unsigned char *)organizationalUnit.c_str(), -1, -1, 0) != 1)
        break;
    if (commonName.length() > 0)
      if (X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC,
                                     (unsigned char *)commonName.c_str(), -1,
                                     -1, 0) != 1)
        break;
    if (email.length() > 0)
      if (X509_NAME_add_entry_by_txt(name, "emailAddress", MBSTRING_ASC,
                                     (unsigned char *)email.c_str(), -1, -1,
                                     0) != 1)
        break;

    // TODO ... set stuff
    /*
    Also set CN in subjectAltName

    x509_extensions     = v3_ca

    [ usr_cert ]
    # Extensions for client certificates (`man x509v3_config`).
    basicConstraints = CA:FALSE
    nsCertType = client, email
    nsComment = "OpenSSL Generated Client Certificate"
    subjectKeyIdentifier = hash
    authorityKeyIdentifier = keyid,issuer
    keyUsage = critical, nonRepudiation, digitalSignature, keyEncipherment
    extendedKeyUsage = clientAuth, emailProtection

    [ server_cert ]
    # Extensions for server certificates (`man x509v3_config`).
    basicConstraints = CA:FALSE
    nsCertType = server
    nsComment = "OpenSSL Generated Server Certificate"
    subjectKeyIdentifier = hash
    authorityKeyIdentifier = keyid,issuer:always
    keyUsage = critical, digitalSignature, keyEncipherment
    extendedKeyUsage = serverAuth
    */

    // Set the certificate's public key from the private key object
    if (!X509_REQ_set_pubkey(pX509Req, pKey)) {
      err("Failed to set public key (X509_REQ_set_pubkey())");
      break;
    }

    if (!X509_REQ_sign(pX509Req, pKey, signatureAlgorithm)) {
      err("Failed to set signing key (X509_REQ_sign())");
      break;
    }

    result = true;
  } while (false);

  if (!result) {
    if (pX509Req) {
      X509_REQ_free(pX509Req);
    }

    return nullptr;
  }

  return pX509Req;
}

bool CertUtils::saveKey(EVP_PKEY *pKey, const std::string &keyName) const {
  if (!pKey) {
    err("Don't you dare throw a null pointer at me, you shall not parse!");
    return false;
  }

  if (keyName.length() <= 5) {
    err("Filename for key must be longer than 5 characters, you entered "
        << keyName);
    return false;
  }

  if (access(keyName.c_str(), F_OK) == 0) {
    err("File " << keyName
                << " exists, will not override an existing private key");
    return false;
  }

  FILE *keyFile = fopen(keyName.c_str(), "wb");
  if (!keyFile) {
    err("Could not open file " << keyName);
    return false;
  }

  int result = PEM_write_PrivateKey(
      keyFile, pKey,
      NULL,  // EVP_des_ede3_cbc() default cipher for encrypting the key on disk
      NULL,  // passphrase required for decrypting the key on disk
      0,     // length of the passphrase string
      NULL,  // callback for requesting a password
      NULL); // data to pass to the callback
  fclose(keyFile);

  if (!result) {
    err("Could not write to file " << keyName);
    return false;
  }

  return true;
}

bool CertUtils::saveCsr(X509_REQ *pReq, const std::string &reqName) const {
  if (!pReq) {
    err("Don't you dare throw a null pointer at me, you shall not parse!");
    return false;
  }

  if (reqName.length() <= 5) {
    err("Filename for key must be longer than 5 characters, you entered "
        << reqName);
    return false;
  }

  if (access(reqName.c_str(), F_OK) == 0) {
    err("File " << reqName
                << " exists, will not override an existing certificate signing "
                   "request");
    return false;
  }

  FILE *reqFile = fopen(reqName.c_str(), "wb");
  if (!reqFile) {
    err("Could not open file " << reqName);
    return false;
  }

  int result = PEM_write_X509_REQ(reqFile, pReq);
  fclose(reqFile);

  if (!result) {
    err("Could not write to file " << reqName);
    return false;
  }

  return true;
}

int pass_cb(char *buf, int size, int rwflag, void *u) {
    (void) rwflag;
    (void) u;

    std::string x = "test";

    int length = x.length();
    length = (length > size) ? size : x.length();

    memcpy(buf, x.c_str(), length);

    return length;
}

bool CertUtils::loadCaCertAndKey(const std::string &certFile, X509 **cert, const std::string &keyFile, EVP_PKEY **key) {
    if (!key || !cert) {
      err("Don't you dare throw a null pointer at me, you shall not parse!");
      return false;
    }

    bool result = false;
    BIO *bio = nullptr;

    do {
        bio = BIO_new(BIO_s_file());
        if (!bio) {
            err("Could not create BIO object (BIO_new())");
            break;
        }

        if (!BIO_read_filename(bio, certFile.c_str())) {
            err("Failed to open CA certificate file " << certFile << " (BIO_read_filename())");
            break;
        }

        *cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
        if (!*cert) {
            err("Failed to read CA certifcate (PEM_read_bio_X509())");
            break;
        }

        BIO_free_all(bio);

        bio = BIO_new(BIO_s_file());
        if (!bio) {
            err("Could not create BIO object (BIO_new())");
            break;
        }

        if (!BIO_read_filename(bio, keyFile.c_str())) {
            err("Failed to open CA key file " << keyFile << " (BIO_read_filename())");
            break;
        }

        *key = PEM_read_bio_PrivateKey(bio, NULL, pass_cb, NULL);
        if (!*key) {
            err("Failed to read CA key (PEM_read_bio_PrivateKey())");
            break;
        }

        BIO_free_all(bio);

        result = true;
    } while (0);

    if (!result) {
        if (bio)
            BIO_free_all(bio);
        if (*cert)
            X509_free(*cert);
        if (*key)
            EVP_PKEY_free(*key);
        return false;
    }

    return true;
}

X509* CertUtils::signReq(EVP_PKEY *caKey, const X509 * const caCrt, X509_REQ *req, size_t daysValid, const EVP_MD *signatureAlgorithm) {
    if (!caKey || !caCrt || !req) {
      err("Don't you dare throw a null pointer at me, you shall not parse!");
      return nullptr;
    }

    bool result = false;
    X509* crt = nullptr;
    do {
        crt = X509_new();
        if (!crt) {
            err("Could not create X509 object (X509_new())");
            break;
        }

        if (!generateAndSetSerial(crt)) {
            err("Could not create create and set serial (generateAndSetSerial())");
            break;
        }

        X509_set_issuer_name(crt, X509_get_subject_name(caCrt));

        X509_gmtime_adj(X509_get_notBefore(crt), 0);
        X509_gmtime_adj(X509_get_notAfter(crt), (long) daysValid * 3600);

        X509_set_subject_name(crt, X509_REQ_get_subject_name(req));

        EVP_PKEY *reqPubkey = X509_REQ_get_pubkey(req);
        X509_set_pubkey(crt, reqPubkey);
        EVP_PKEY_free(reqPubkey);

        if (X509_sign(crt, caKey, signatureAlgorithm) == 0) {
            err("Could not sign certificate (X509_sign())");
            break;
        }

        result = true;
    } while (0);

    if (!result) {
        if (crt)
            X509_free(crt);
    }

    // TODO During development
    if (crt) {
        BIO* bio = BIO_new_fp(stdout, BIO_NOCLOSE);
        X509_print_ex(bio, crt, XN_FLAG_COMPAT, X509_FLAG_COMPAT);
        BIO_free_all(bio);
    }

    return crt;
}

bool CertUtils::generateAndSetSerial(X509 *crt) {
    if (crt == nullptr)
        return false;

    unsigned char bytes[20];
    if (RAND_bytes(bytes, sizeof(bytes)) != 1)
        return false;

    // Ensure positive number
    bytes[0] &= 0x7f;

    BIGNUM *b = BN_new();
    BN_bin2bn(bytes, sizeof(bytes), b);
    ASN1_INTEGER *serial = ASN1_INTEGER_new();
    BN_to_ASN1_INTEGER(b, serial);

    X509_set_serialNumber(crt, serial);

    ASN1_INTEGER_free(serial);
    BN_free(b);

    return true;
}

std::string CertUtils::printCert(const std::string &certFile) const {
    BIO *bio = nullptr;
    X509 *cert = nullptr;

    do {
        bio = BIO_new(BIO_s_file());
        if (!bio) {
            err("Could not create BIO object (BIO_new())");
            break;
        }

        if (!BIO_read_filename(bio, certFile.c_str())) {
            err("Failed to open CA certificate file " << certFile << " (BIO_read_filename())");
            break;
        }

        cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
        if (!cert) {
            err("Failed to read CA certifcate (PEM_read_bio_X509())");
            break;
        }

        if (bio)
            BIO_free_all(bio);

        bio = BIO_new_fp(stdout, BIO_NOCLOSE);
        X509_print_ex(bio, cert, XN_FLAG_COMPAT, X509_FLAG_COMPAT);
    } while (0);

    if (cert)
        X509_free(cert);

    if (bio)
        BIO_free_all(bio);

    return std::string("todo...");
}
