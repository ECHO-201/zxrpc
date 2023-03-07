#pragma once

#include <string>
using std::string;

#include <vector>
#include <map>

namespace zxrpc {
namespace xml {

class Xml
{
public:
    Xml();
    Xml(const string& name);
    Xml(const string& name, const string& text);
    ~Xml();

    const string& name() const;

    void name(const string& name);

    const string& text() const;

    void text(const string& text);

    string& attr(const string & key);

    void attr(const string& key, const string& value);

    const Xml& operator [] (int index) const;

    const Xml& operator [] (const string& name) const;

    void append(const Xml& child);

    int size();

    void clear();
    
    string toString() const;

    typedef std::vector<Xml>::iterator iterator;
    typedef std::vector<Xml>::const_iterator const_iterator;

    iterator begin()
    {
        return m_element.begin();
    }

    iterator end()
    {
        return m_element.end();
    }

    const_iterator begin() const
    {
        return m_element.begin();
    }

    const_iterator end() const
    {
        return m_element.end();
    }

    static Xml const& null();

private:
    string m_name;
    string m_text;
    std::vector<Xml> m_element;
    std::map<string, string> m_attrs;
};

}}
