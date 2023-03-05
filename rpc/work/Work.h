#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include "Context.h"
#include "Plugin.h"

namespace zxrpc {
namespace work {

class Work
{
public:
    Work();
    Work(const string &name, bool flag);
    ~Work();

    void append(Plugin *plugin);

    void set_name(const string &name);
    const string& get_name() const;

    void set_switch(bool flag);
    bool get_switch() const;

    bool run(Context &ctx);

protected:
    string m_name;
    bool m_switch_flag;
    vector<Plugin *> m_plugins;
};

}}
