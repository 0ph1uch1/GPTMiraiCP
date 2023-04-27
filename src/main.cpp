// MiraiCP依赖文件(只需要引入这一个)


#include "CPPPlugin.h"
#include "utils.h"
#include "Sessions.h"
#include "communication.h"
#include "json.hpp"
#include "Event.h"
#include <iostream>
#include <fstream>
#include "Events/PrivateMessageEvent.h"
#include "GPTCalls.h"
#include "Events/GroupMessageEvent.h"

using namespace MiraiCP;

const PluginConfig CPPPlugin::config{
        "Plugin id",          // 插件id，如果和其他插件id重复将会被拒绝加载！
        "Plugin name",        // 插件名称
        "Version",            // 插件版本
        "Author name",        // 插件作者
        "Plugin description",  // 可选：插件描述
        "Publish time"        // 可选：日期
};


void init() {
    std::ifstream i;
    i.open("./gpt_config.json", std::ios::in);
    nlohmann::json j;
    std::cout << "Reading config file..." << std::endl;
    try {
        i >> j;
        communication::setPort(j["port"]);
        Sessions::init(j["permitted"]);
    } catch (std::exception &e) {
        std::cerr << "Failed to read: ./permitted.json" << std::endl;
        throw e;
    }
}

// 插件实例
class PluginMain : public CPPPlugin {
public:
    // 配置插件信息
    PluginMain() : CPPPlugin() {}

    ~PluginMain() override = default; // override关键字是为了防止内存泄漏

    // 入口函数。插件初始化时会被调用一次，请在此处注册监听
    void onEnable() override {
        init();

        MiraiCP::Event::registerEvent<MiraiCP::PrivateMessageEvent>([](const MiraiCP::PrivateMessageEvent &e) {
            auto msgchain = e.message.filter<PlainText>();
            if (msgchain.empty()) return;
            auto msg = msgchain[0].content;
            for (size_t i = 1; i < msgchain.size(); i++) {
                msg += msgchain[i].content;
            }
            auto reply = GPTCalls::call(e.from()->id(), msg);
            if (reply.empty()) return;
            e.chat()->sendMessage(reply);
        });
    }

    // 退出函数。请在这里结束掉所有子线程，否则可能会导致程序崩溃
    void onDisable() override {
        /*插件结束前执行*/
    }
};

// 创建当前插件实例。请不要进行其他操作，
// 初始化请在onEnable中进行
void MiraiCP::enrollPlugin() {
    MiraiCP::enrollPlugin<PluginMain>();
}
