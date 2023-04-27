//
// Created by antares on 4/26/23.
//


#include "Sessions.h"
#include <unordered_set>
#include <fstream>
#include "json.hpp"
#include <iostream>
#include <mutex>
#include "communication.h"

long long Sessions::getSessionId(MiraiCP::QQID qqid) {
    std::shared_lock lock(mtx);
    auto it = sessions.find(qqid);
    if (it == sessions.end()) {
        return -1;
    }
    if (it->second == -1) {
        lock.unlock();
        getNewSession(qqid);
        lock.lock();
        it = sessions.find(qqid);
        return it->second;
    }
    return it->second;
}


void Sessions::getNewSession(MiraiCP::QQID qqid) {
    std::unique_lock lock(mtx);
    auto it = sessions.find(qqid);
    if (it == sessions.end()) {
        return;
    }
    it->second = communication::getNewSession(qqid);
    // write to file
    try {
        std::ifstream in("./gpt_config.json", std::ios::in);
        nlohmann::json j;
        in >> j;
        {
            std::vector<nlohmann::json> v;
            for (auto &it2: sessions) {
                v.push_back({{"qqid", it2.first},
                             {"sid",  it2.second}});
            }
            j["permitted"] = v;
        }
        std::ofstream out("./gpt_config.json", std::ios::out);
        out << j.dump(4);
    } catch (std::exception &e) {
        std::cerr << "Error writing to file: " << e.what() << std::endl;
    }
}

void Sessions::init(const nlohmann::json &j) {
    auto &sessions = getSessions();

    for (auto &it: j) {
        sessions.sessions[it["qqid"]] = it["sid"];
    }
}

Sessions &Sessions::getSessions() {
    static Sessions allowedIDs;
    return allowedIDs;
}
