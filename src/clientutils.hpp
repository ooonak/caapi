#ifndef __CLIENTUTILS_HPP__
#define __CLIENTUTILS_HPP__

#include "certutils.hpp"
#include <string>

struct x509Conf {
  std::string country;
  std::string stateOrProvinceName;
  std::string localityName;
  std::string organizationName;
  std::string organizationalUnit;
  std::string commonName;
  std::string email;
};

class ClientUtils : public CertUtils {
public:
  /**
   * @brief makeCsr
   * @param config
   * @param keyFileName
   * @param csrFileName
   * @return false on error
   * Note: Use 'openssl req -in req.pem -text -noout' to see generated
   * certificate signing request in console
   */
  bool makeCsr(const x509Conf &config,
               const std::string &keyFileName = std::string("key.pem"),
               const std::string &csrFileName = std::string("req.pem"));

  /*
  bool verifyCert(const std::string &certFileName = std::string("cert.pem"),
                  const std::string &caFileName = std::string("ca.pem"));
  */
};

#endif // __CLIENTUTILS_HPP__
