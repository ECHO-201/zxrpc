#pragma once

#include "Plugin.h"
#include "Context.h"
using namespace zxrpc::work;

namespace zxrpc {
    namespace plugin {
        class EchoPlugin : public Plugin
        {
        public:
            EchoPlugin();
            virtual ~EchoPlugin();
            virtual bool run(Context & ctx);
        };

        PLUGINCORE(EchoPlugin)
    }
}
