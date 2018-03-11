#ifndef __CAUTILS_HPP__
#define __CAUTILS_HPP__

#include "certutils.hpp"

class CaUtils : public CertUtils {
public:
  explicit CaUtils(const std::string &workFolder = std::string("ca"),
                   const std::string &caCertFile = std::string("caCert.pem"),
                   const std::string &caKeyFile = std::string("caKey.pem"));
    ~CaUtils();
    bool init();

    //bool signRequest();

private:
    const std::string workFolder_;
    const std::string caCertFile_;
    const std::string caKeyFile_;
    EVP_PKEY *caKey_ = nullptr;
    X509 *caCert_ = nullptr;

};

#endif // __CAUTILS_HPP__
