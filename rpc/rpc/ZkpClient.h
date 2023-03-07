#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>
namespace zxrpc {
namespace rpc {

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
    void Create(const char *path, const char *data, int datalen, int state=0);

    /**
     * @brief 根据参数指定的znode节点路径，或者znode节点的值
     */
    std::string GetData(const char *path);
private:

    /**
     * zk的客户端句柄
     */
    zhandle_t *m_zkpdle;
};

}}