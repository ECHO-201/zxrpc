#pragma once
#include <iostream>
#include <string>
using namespace std;

#include "Xml.h"

namespace zxrpc {
namespace xml {

class Document
{
public:
    Document();
    ~Document();

    void load_file(const string& filename);
    void load_string(const string& str);
    void skip_white_spaces();
    Xml parse();
    
private:
    bool parse_declaration();
    bool parse_comment();
    string parse_element_name();
    string parse_element_text();
    string parse_element_attr_key();
    string parse_element_attr_val();

private:
    string m_str;
    unsigned long m_index;
};

}}
