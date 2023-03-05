#pragma once

#include "Plugin.h"
#include "Context.h"
using namespace zxrpc::work;

namespace zxrpc {
    namespace plugin {
        class TestPlugin : public Plugin
        {
        public:
            TestPlugin();
            virtual ~TestPlugin();
            virtual bool run(Context & ctx);
        };
        PLUGINCORE(TestPlugin)
    }
}
