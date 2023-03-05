#pragma once

#include <string>
#include <vector>
#include <map>


//定义一个存放配置信息的map
//key 是string 存放一个标题section
//value 是一个map 存放该标题下面的所有key-value键值对
namespace zxrpc {
namespace tools {
typedef std::map<std::string, std::map<std::string, std::string> *> STR_MAP;

typedef STR_MAP::iterator STR_MAP_ITER;

class config_file {
public:
    ~config_file();

    /**
     * @brief 获取字符串类型配置信息
     */
    std::string GetStringInfo(const std::string& section, const std::string& key, const std::string& default_value = "");

    /**
     * @brief 字符串集合配置信息
     */
    std::vector<std::string> GetStringListInfo(const std::string& section, const std::string& key);

    /**
     * @brief 获取整型类型配置信息
     */
    unsigned GetNumberInfo(const std::string& section, const std::string& key, unsigned default_value = 0);

    /**
     * @brief 获取布尔类型配置信息
     */
    bool GetBoolInfo(const std::string& section, const std::string& key, bool default_value = false);

    /**
     * @brief 获取浮点类型配置信息
     */
    float GetFloatInfo(const std::string& section, const std::string& key, const float& default_value);

    //设置配置文件所在路径
    static bool setPath(const std::string& path);
    
    //获取单例
    static config_file *instance();

private:
    config_file() { };

    // 字符串配置文件解析基础方法
    bool isSection(std::string line, std::string& section);
    unsigned parseNumber(const std::string& s);
    std::string trimLeft(const std::string& s);
    std::string trimRight(const std::string& s);
    std::string trim(const std::string& s);
    bool Load(const std::string& path);

    static config_file *config;

    STR_MAP _map;
};
}
}