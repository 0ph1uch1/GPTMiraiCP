//
// Created by antares on 4/27/23.
//

#include "GPTCalls.h"
#include "Sessions.h"
#include "communication.h"

std::string GPTCalls::call(MiraiCP::QQID qqid, const std::string &message) {
    auto sid = Sessions::getSessions().getSessionId(qqid);
    if (sid == -1) { return ""; }
    int maxRetry = 5;
    while (maxRetry--) {
        try {
            return communication::call(sid, message);
        } catch (std::exception &e) {
            Sessions::getSessions().getNewSession(qqid);
            sid = Sessions::getSessions().getSessionId(qqid);
            if (sid == -1) { return ""; }
        }
    }
    return "";
}
