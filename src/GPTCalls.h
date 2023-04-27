//
// Created by antares on 4/27/23.
//

#ifndef MIRAICP_TEMPLATE_GPTCALLS_H
#define MIRAICP_TEMPLATE_GPTCALLS_H

#include <string>
#include "SdkType.h"

class GPTCalls {
public:
    static std::string call(MiraiCP::QQID qqid, const std::string &message);
};


#endif //MIRAICP_TEMPLATE_GPTCALLS_H
