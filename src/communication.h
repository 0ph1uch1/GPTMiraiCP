//
// Created by antares on 4/27/23.
//

#ifndef MIRAICP_TEMPLATE_COMMUNICATION_H
#define MIRAICP_TEMPLATE_COMMUNICATION_H


#include "SdkType.h"
#include <string>

struct communication {
    static long long getNewSession(MiraiCP::QQID qqid);

    static std::string call(long long session, const std::string &message, const std::string &systemMsg = "",
                            const std::string &model = "");

    static void setPort(int port);

    static int getPort();
};


#endif //MIRAICP_TEMPLATE_COMMUNICATION_H
