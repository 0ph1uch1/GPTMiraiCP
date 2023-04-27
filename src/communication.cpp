//
// Created by antares on 4/27/23.
//

#include "communication.h"
#include "httplib.h"
#include <json.hpp>


static int port = 27000;

std::string communication::call(long long session, const std::string &message, const std::string &systemMsg,
                                const std::string &model) {
    httplib::Client client("127.0.0.1", port);
    client.set_keep_alive(true);
    client.set_connection_timeout(300);
    client.set_read_timeout(300);
    nlohmann::json j{
            {"sid",       session},
            {"msg",       message},
            {"ensure_id", true}
    };
    if (!systemMsg.empty()) {
        j["systemMsg"] = systemMsg;
    }
    if (!model.empty()) {
        j["model"] = model;
    }
    auto res = client.Post("/api", j.dump(), "application/json");
    if (!res || res->status >= 400) {
        std::cerr << res.error() << std::endl;
        throw std::runtime_error("Error calling API");
    }
    return res->body;
}

void communication::setPort(int inPort) {
    port = inPort;
}

long long communication::getNewSession(MiraiCP::QQID qqid) {
    httplib::Client client("127.0.0.1", port);
    auto res = client.Get("/newid?hint=" + std::to_string(qqid));
    const int maxretries = 5;
    auto retry = maxretries;
    while (!res && retry--) {
        res = client.Get("/newid?hint=" + std::to_string(qqid));
    }
    if (!res || res->status != 200) {
        throw std::runtime_error("Error calling API");
    }

    auto sid = std::stoll(res->body);
    res = client.Get("/create?sid=" + std::to_string(sid));
    retry = maxretries;
    while (!res && retry--) {
        res = client.Get("/create?sid=" + std::to_string(sid));
    }
    if (!res || res->status != 200) {
        throw std::runtime_error("Error calling API");
    }
    return sid;
}

int communication::getPort() {
    return port;
}
