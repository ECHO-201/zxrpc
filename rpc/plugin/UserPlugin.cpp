#include "UserPlugin.h"
#include<jsoncpp/json/json.h>
using namespace zxrpc::work;
using namespace zxrpc::plugin;

UserPlugin::UserPlugin() : Plugin()
{

}

UserPlugin::~UserPlugin()
{

}

bool UserPlugin::run(Context& ctx)
{
    string &input = ctx.ref<string>("input");

    Json::Reader reader;
    Json::Value value;
    reader.parse(input, value);

    ctx.ref<string>("output") += "hello, " + value["name"].asString() + " user plugin run!\n";
    return true; // 是否走下一个工作流
}

