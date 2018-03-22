#ifndef __SERVER_HPP__
#define __SERVER_HPP__

class CaUtils;

class Server {
public:
    explicit Server();
    ~Server();
    void run();
private:
    bool initialized_;
    CaUtils *caUtils_;
};

#endif // __SERVER_HPP__
