#ifndef __CAUTILS_HPP__
#define __CAUTILS_HPP__

#include "certutils.hpp"
#include <string>
#include <sstream>

class CaUtils : public CertUtils {
public:
  explicit CaUtils(const std::string &workFolder = std::string("ca"),
                   const std::string &caCertFile = std::string("ca_cert.pem"),
                   const std::string &caKeyFile = std::string("ca_key.pem"));
    ~CaUtils();
    bool init();
    bool sign(std::ostringstream &out, const std::string &csr);

private:
    const std::string workFolder_;
    const std::string caCertFile_;
    const std::string caKeyFile_;
    EVP_PKEY *caKey_ = nullptr;
    X509 *caCert_ = nullptr;

};

#endif // __CAUTILS_HPP__
