#include "TestPlugin.h"
#include<jsoncpp/json/json.h>

using namespace zxrpc::work;
using namespace zxrpc::plugin;


TestPlugin::TestPlugin() : Plugin()
{

}

TestPlugin::~TestPlugin()
{

}

bool TestPlugin::run(Context & ctx)
{
    string& input = ctx.ref<string>("input");

    // Json::Reader reader;
    // Json::Value value;
    // reader.parse(input, value);

    // ctx.ref<string>("output") += "hello, " + value["name"].asString() + " test plugin run!\n";
    return true;
}

