//
// Created by antares on 4/26/23.
//

#ifndef MIRAICP_TEMPLATE_SESSIONS_H
#define MIRAICP_TEMPLATE_SESSIONS_H

#include <unordered_map>
#include "SdkType.h"
#include <cstddef>
#include <shared_mutex>
#include <json_fwd.hpp>

struct Sessions {
    std::unordered_map<MiraiCP::QQID, long long> sessions;
    std::shared_mutex mtx;

    long long getSessionId(MiraiCP::QQID qqid);

    void getNewSession(MiraiCP::QQID qqid);
static Sessions& getSessions();
    static void init(const nlohmann::json& j);
};


#endif //MIRAICP_TEMPLATE_SESSIONS_H
