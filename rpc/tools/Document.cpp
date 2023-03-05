#include <ctype.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include "Document.h"
using namespace zxrpc::xml;

Document::Document() : m_index(0)
{
}

Document::~Document()
{
}

void Document::load_file(const string & filename)
{
    std::ifstream in(filename.c_str());
    ostringstream buf;
    char ch;
    while(in.get(ch))
    {
        buf.put(ch);
    }
    load_string(buf.str());
}

void Document::load_string(const string & str)
{
    m_str = str;
    m_index = 0;
}

void Document::skip_white_spaces()
{
    while (m_str[m_index] == ' ' || m_str[m_index] == '\r' || m_str[m_index] == '\n' || m_str[m_index] == '\t')
        m_index++;
}

Xml Document::parse()
{
    skip_white_spaces();
    if (m_index == m_str.size() || m_str[m_index] == '\0')
    {
        throw std::logic_error("xml document is empty");
    }

    // parse xml declaration
    if (m_str[m_index + 0] == '<' &&
        m_str[m_index + 1] == '?' &&
        m_str[m_index + 2] == 'x' &&
        m_str[m_index + 3] == 'm' &&
        m_str[m_index + 4] == 'l')
    {
        if (!parse_declaration())
        {
            throw std::logic_error("xml declaration is error");
        }
    }

    skip_white_spaces();

    // parse xml comment
    if (m_str[m_index + 0] == '<' &&
        m_str[m_index + 1] == '!' &&
        m_str[m_index + 2] == '-' &&
        m_str[m_index + 3] == '-')
    {
        if (!parse_comment())
        {
            throw std::logic_error("xml comment is error");
        }
    }

    skip_white_spaces();

    Xml xml;
    string name;

    if ((m_str[m_index + 0] == '<') && (isalpha(m_str[m_index + 1]) || m_str[m_index + 1] == '_'))
    {
        m_index++;

        // parse elem's name
        name = parse_element_name();
        xml.name(name);

        skip_white_spaces();

        while (m_str[m_index] != '\0')
        {
            // empty tag
            if (m_str[m_index + 0] == '/')
            {
                if (m_str[m_index + 1] == '>')
                {
                    m_index += 2;
                    return xml;
                }
                else
                {
                    throw std::logic_error("xml empty Xml is error");
                }
            }
            else if (m_str[m_index + 0] == '<')
            {
                if (m_str[m_index + 1] == '/')
                {
                    // find the end tag
                    string end_tag = "</" + name + ">";
                    size_t pos = m_str.find(end_tag, m_index);
                    if (pos == std::string::npos)
                    {
                        throw std::logic_error("xml Xml " + name + " end tag not found");
                    }
                    if (pos == m_index)
                    {
                        m_index += end_tag.size();
                        return xml;
                    }
                }
                else if (m_str[m_index + 0] == '<' &&
                         m_str[m_index + 1] == '!' &&
                         m_str[m_index + 2] == '-' &&
                         m_str[m_index + 3] == '-')
                {
                    if (!parse_comment())
                    {
                        throw std::logic_error("xml comment is error");
                    }
                }
                else
                {
                    Xml elem = parse();
                    xml.append(elem);
                }
            }
            else if (m_str[m_index] == '>')
            {
                m_index++;
                string text = parse_element_text();
                skip_white_spaces();
                if (text != "")
                {
                    xml.text(text);
                }
                else
                {
                    Xml child = parse();
                    xml.append(child);
                }
            }
            else
            {
                string key = parse_element_attr_key();

                skip_white_spaces();

                if (m_str[m_index] != '=')
                {
                    throw std::logic_error("xml Xml attr is error" + key);
                }
                ++m_index;
                string val = parse_element_attr_val();
                xml.attr(key, val);
            }

            skip_white_spaces();
        }
    }
    return xml;
}

bool Document::parse_declaration()
{
    size_t pos = m_str.find("?>", m_index);
    if (pos == std::string::npos)
    {
        return false;
    }
    m_index = pos + 2;
    return true;
}

bool Document::parse_comment()
{
    size_t pos = m_str.find("-->", m_index);
    if (pos == std::string::npos)
    {
        return false;
    }
    m_index = pos + 3;
    return true;
}

string Document::parse_element_name()
{
    skip_white_spaces();

    string out;
    if (isalpha(m_str[m_index]) || (m_str[m_index] == '_'))
    {
        out += m_str[m_index];
        m_index++;
        while ((m_str[m_index] != '\0') &&
            (isalnum(m_str[m_index]) || (m_str[m_index] == '_') || (m_str[m_index] == '-') || (m_str[m_index] == ':')))
        {
            out += m_str[m_index];
            m_index++;
        }
    }
    return out;
}

string Document::parse_element_text()
{
    skip_white_spaces();

    string out;
    while (m_str[m_index] != '<')
    {
        out += m_str[m_index];
        m_index++;
    }
    return out;
}

string Document::parse_element_attr_key()
{
    skip_white_spaces();

    string out;
    if (isalpha(m_str[m_index]) || (m_str[m_index] == '_'))
    {
        out += m_str[m_index];
        ++m_index;
        while (isalnum(m_str[m_index]) || (m_str[m_index] == '_') || (m_str[m_index] == '-') || (m_str[m_index] == ':'))
        {
            out += m_str[m_index];
            m_index++;
        }
    }
    return out;
}

string Document::parse_element_attr_val()
{
    skip_white_spaces();

    if (m_str[m_index] != '"')
    {
        throw std::logic_error("xml Xml attr value should be in double quotes");
    }
    m_index++;

    string out;
    while (m_str[m_index] != '"')
    {
        out += m_str[m_index];
        ++m_index;
    }

    ++m_index;
    return out;
}