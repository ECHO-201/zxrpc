#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

namespace watcher {
namespace zookeeper {

// 封装zk客户端类
class ZkpClient
{
public:
    ZkpClient();
    ~ZkpClient();

    /**
     * @brief zkclient启动连接zkserver
     */
    void Start();

    /**
     * @brief 在zkserver上根据指定的path创建znode节点
     */
    void Create(const char *path, const char *data, int datalen, int state = 0);

    /**
     * @brief 根据参数指定的znode节点路径，或者znode节点的值
     */
    std::string GetData(const char *path);

    /**
     * @brief 获取所有子节点
     */
    bool GetChildren(const char *path, bool watch_cast, String_vector &s_v);

    /**
     * @brief 删除节点
     */
    bool DelNode(const char *path);
    
private:

    /**
     * zk的客户端句柄
     */
    zhandle_t *m_zkpdle;
};

}}
