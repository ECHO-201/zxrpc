#include <sstream>

#include "Xml.h"
using namespace zxrpc::xml;

Xml::Xml()
{
}

Xml::Xml(const string& name) : m_name(name)
{
}

Xml::Xml(const string& name, const string& text) : m_name(name), m_text(text)
{
}

Xml::~Xml()
{
}

const string& Xml::name() const
{
    return m_name;
}

const string& Xml::text() const
{
    return m_text;
}

void Xml::name(const string& name)
{
    m_name = name;
}

void Xml::text(const string& text)
{
    m_text = text;
}

string& Xml::attr(const string& key)
{
    return m_attrs[key];
}

void Xml::attr(const string& key, const string& value)
{
    if (value != "")
    {
        m_attrs[key] = value;
        return;
    }

    std::map<string, string>::iterator it = m_attrs.find(key);
    if (it != m_attrs.end())
    {
        m_attrs.erase(it);
    }
}

const Xml& Xml::operator [] (int index) const
{
    int size = m_element.size();
    if (index >= 0 && index < size)
    {
        return m_element.at(index);
    }
    return null();
}

const Xml& Xml::operator [] (const string& name) const
{
    for (Xml::const_iterator it = m_element.begin(); it != m_element.end(); it++)
    {
        if ((*it).name() == name)
        {
            return (*it);
        }
    }
    return null();
}

void Xml::append(const Xml& child)
{
    m_element.push_back(child);
}

int Xml::size()
{
    return m_element.size();
}

void Xml::clear()
{
    m_name.clear();
    m_text.clear();
    m_attrs.clear();
    m_element.clear();
}

string Xml::toString() const
{
    if (m_name == "")
    {
        return "";
    }
    std::ostringstream os;
    os << "<" << m_name;
    for (std::map<string, string>::const_iterator it = m_attrs.begin(); it != m_attrs.end(); it++)
    {
        os << " " << it->first << "=\"" << it->second << "\"";  
    }
    os << ">";
    if (m_element.size() > 0)
    {
        for (std::vector<Xml>::const_iterator it = m_element.begin(); it != m_element.end(); it++)
        {
            os << (*it).toString();
        }
    }
    else
    {
        os << m_text;
    }
    os << "</" << m_name << ">";
    return os.str();
}

Xml const& Xml::null()
{
    static const Xml null;
    return null;
}
