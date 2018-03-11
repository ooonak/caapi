#include "cautils.hpp"

CaUtils::CaUtils(const std::string &workFolder, const std::string &caCertFile, const std::string &caKeyFile)
    : workFolder_(workFolder), caCertFile_(caCertFile), caKeyFile_(caKeyFile) {
}

CaUtils::~CaUtils() {
    dbg("dtor, cleaning up");
    if (caCert_)
        X509_free(caCert_);
    if (caKey_)
        EVP_PKEY_free(caKey_);
}

bool CaUtils::init() {
    if (loadCaCertAndKey(caCertFile_, &caCert_, caKeyFile_, &caKey_)) {
        dbg("Loaded CA cert and key");
        return true;
    }
    else {
        return false;
    }
}
