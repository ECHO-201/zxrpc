#include "EchoPlugin.h"
using namespace zxrpc::work;
using namespace zxrpc::plugin;

#include "Logger.h"

#include "Singleton.h"
#include "System.h"
using namespace zxrpc::tools;

EchoPlugin::EchoPlugin() : Plugin()
{
    
}

EchoPlugin::~EchoPlugin()
{

}

bool EchoPlugin::run(Context & ctx)
{
    string &input = ctx.ref<string>("input");
    ctx.ref<string>("output") = input;
    return true;
}