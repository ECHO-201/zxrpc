#pragma once
#include <map>
#include <string>
using std::string;

#include "Document.h"
#include "Xml.h"
using namespace zxrpc::xml;

#include "Work.h"

namespace zxrpc {
namespace work {

class Workflow
{
public:
    Workflow();
    ~Workflow();

    bool load(const string &workinfo);
    bool run(const string &work, const string &input, string &output);

    void clear();
    
    int get_work_size();

private:
    bool load_plugin(Work* work, Xml &elem);

private:
    std::map<string, Work *> m_works;
};

}}
