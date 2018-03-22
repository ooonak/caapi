#include "server.hpp"
#include "cautils.hpp"
#include "crow_all.h"
#include <sstream>

Server::Server() : initialized_(false) {
    std::cout << "Server ctor" << std::endl;

    caUtils_ = new CaUtils("ca", "ca/intermediate.cert.pem", "ca/intermediate.key.pem");
    initialized_ = caUtils_->init();
}

Server::~Server() {
    std::cout << "Server dtor" << std::endl;
}

void Server::run() {
    if (!initialized_)
        return;

    crow::SimpleApp app;

    CROW_ROUTE(app, "/sign").methods("POST"_method)([this](const crow::request &req) {
        crow::json::wvalue res;

        auto val = crow::json::load(req.body);
        if (!val) {
            res["error"] = "Empty body";
            return crow::response(400, res);
        }

        if (!val.has("csr")) {
            res["error"] = "No csr";
            return crow::response(400, res);
        }

        auto csr = val["csr"].s();
        std::ostringstream out;

        if (!caUtils_->sign(out, csr)) {
            res["error"] = out.str();
            return crow::response(400, res);
        }

        res["cert"] = out.str();
        return crow::response(201, res);
    });

    crow::logger::setLogLevel(crow::LogLevel::Info);
    app.port(8080).run();
}
