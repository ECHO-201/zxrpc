#pragma once

#include "Plugin.h"
#include "Context.h"
using namespace zxrpc::work;

namespace zxrpc {
    namespace plugin {
        class UserPlugin : public Plugin
        {
        public:
            UserPlugin();
            virtual ~UserPlugin();
            virtual bool run(Context & ctx);
        };
        PLUGINCORE(UserPlugin)
    }
}
