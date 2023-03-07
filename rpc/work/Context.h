#pragma once
#include <string>
#include <map>

namespace zxrpc {
namespace work {

class Object
{
public:
    Object() {}
    virtual ~Object() {}
};


class Context
{
public:
    Context() {}
    ~Context() { clear(); }

    template <typename T>
    void set(const std::string &key, T value);

    template <typename T>
    T get(const std::string &key);

    template <typename T>
    T & ref(const std::string &key);

    void clear();

private:
    std::map<std::string, bool> m_bool;
    std::map<std::string, char> m_char;
    std::map<std::string, short> m_short;
    std::map<std::string, int> m_int;
    std::map<std::string, long> m_long;
    std::map<std::string, double> m_double;
    std::map<std::string, std::string> m_str;
    std::map<std::string, Object *> m_obj;
};

template<>
inline void Context::set<bool>(const std::string &key, bool value)
{
    m_bool[key] = value;
}

template<>
inline bool Context::get<bool>(const std::string &key)
{
    std::map<std::string, bool>::iterator it = m_bool.find(key);
    if (it != m_bool.end())
        return it->second;

    return false;
}

template <>
inline void Context::set<char>(const std::string &key, char value)
{
    m_char[key] = value;
}

template <>
inline char Context::get<char>(const std::string &key)
{
    std::map<std::string, char>::iterator it = m_char.find(key);
    if (it != m_char.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned char>(const std::string &key, unsigned char value)
{
    m_char[key] = value;
}

template <>
inline unsigned char Context::get<unsigned char>(const std::string &key)
{
    std::map<std::string, char>::iterator it = m_char.find(key);
    if (it != m_char.end())
        return static_cast<unsigned char>(it->second);
    return 0;
}

template <>
inline void Context::set<short>(const std::string &key, short value)
{
    m_short[key] = value;
}

template <>
inline short Context::get<short>(const std::string &key)
{
    std::map<std::string, short>::iterator it = m_short.find(key);
    if (it != m_short.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned short>(const std::string &key, unsigned short value)
{
    m_short[key] = value;
}

template <>
inline unsigned short Context::get<unsigned short>(const std::string &key)
{
    std::map<std::string, short>::iterator it = m_short.find(key);
    if (it != m_short.end())
        return static_cast<unsigned short>(it->second);
    return 0;
}

template <>
inline void Context::set<int>(const std::string &key, int value)
{
    m_int[key] = value;
}

template <>
inline int Context::get<int>(const std::string &key)
{
    std::map<std::string, int>::iterator it = m_int.find(key);
    if (it != m_int.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned int>(const std::string &key, unsigned int value)
{
    m_int[key] = value;
}

template <>
inline unsigned int Context::get<unsigned int>(const std::string &key)
{
    std::map<std::string, int>::iterator it = m_int.find(key);
    if (it != m_int.end())
        return static_cast<unsigned int>(it->second);
    return 0;
}

template <>
inline void Context::set<long>(const std::string &key, long value)
{
    m_long[key] = value;
}

template <>
inline long Context::get<long>(const std::string &key)
{
    std::map<std::string, long>::iterator it = m_long.find(key);
    if (it != m_long.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned long>(const std::string &key, unsigned long value)
{
    m_long[key] = value;
}

template <>
inline unsigned long Context::get<unsigned long>(const std::string &key)
{
    std::map<std::string, long>::iterator it = m_long.find(key);
    if (it != m_long.end())
        return static_cast<unsigned long>(it->second);
    return 0;
}

template <>
inline void Context::set<double>(const std::string &key, double value)
{
    m_double[key] = value;
}

template <>
inline double Context::get<double>(const std::string &key)
{
    std::map<std::string, double>::iterator it = m_double.find(key);
    if (it != m_double.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<const std::string &>(const std::string &key, const std::string &value)
{
    m_str[key] = value;
}

template <>
inline std::string & Context::get<std::string &>(const std::string &key)
{
    return m_str[key];
}

template <>
inline void Context::set<Object *>(const std::string &key, Object* value)
{
    m_obj[key] = value;
}

template <>
inline Object * Context::get<Object *>(const std::string &key)
{
    std::map<std::string, Object *>::iterator it = m_obj.find(key);
    if (it != m_obj.end())
        return it->second;
    return NULL;
}

template <>
inline std::string & Context::ref<std::string>(const std::string &key)
{
    return m_str[key];
}

template <typename T>
inline T & Context::ref(const std::string &key)
{
    std::map<std::string, Object *>::iterator it = m_obj.find(key);
    if (it != m_obj.end())
    {
        return *reinterpret_cast<T *>(it->second);
    }
    else
    {
        m_obj[key] = new T();
        return *reinterpret_cast<T *>(m_obj[key]);
    }
}

inline void Context::clear()
{
    m_bool.clear();
    m_char.clear();
    m_short.clear();
    m_int.clear();
    m_long.clear();
    m_double.clear();
    m_str.clear();
    for (std::map<std::string, Object *>::iterator it = m_obj.begin(); it != m_obj.end(); ++it)
    {
        delete it->second;
        it->second = NULL;
    }
    m_obj.clear();
}

}}
