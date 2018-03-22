#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include <string>

class Client {
public:
    explicit Client(const std::string &url);
    void makeCert();
};

#endif // __CLIENT_HPP__
