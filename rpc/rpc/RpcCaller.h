#pragma once

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

#include "ClientSocket.h"
using namespace zxrpc::socket;

#include "DataStream.h"
using namespace zxrpc::serialize;

namespace zxrpc {
namespace rpc {

struct MsgHead {
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};

const uint32_t send_buff_size = 1024;
const uint32_t recv_buff_size = 1024;

class RpcCaller
{
public:
    RpcCaller();
    ~RpcCaller();

    bool connect();
    void connect(const std::string & ip, int port);

    template <typename R, typename ...Args>
    R call(const string & name, const Args&... args);

    template <typename ...Args>
    void send(const string & name, const Args&... args);

    void get_host_data(const string & method_name);

    void set_service(const string & service_name);
private:
    string m_ip;
    int m_port;
    ClientSocket m_socket;
    std::string m_service;
};

template <typename R, typename ...Args>
R RpcCaller::call(const string & name, const Args&... args)
{
    DataStream in;
    in << name;
    in.write_args(args...);

    MsgHead head;
    strcpy(head.flag, "work");
    head.cmd = 0;
    head.len = in.size();

    char send_buff[send_buff_size];
    std::memcpy(send_buff, (char *)&head, sizeof(MsgHead));
    std::memcpy(send_buff + sizeof(MsgHead), in.data(), in.size());

    int len = sizeof(MsgHead) + in.size();
    m_socket.send(send_buff, len);

    char recv_buff[recv_buff_size];
    int recv_size = m_socket.recv(recv_buff, recv_buff_size);

    DataStream out;
    out.write(recv_buff, recv_size);

    R v;
    out >> v;
    return v;
}

template <typename ...Args>
void RpcCaller::send(const string & name, const Args&... args)
{
    DataStream in;
    in << name;
    in.write_args(args...);

    MsgHead head;
    strcpy(head.flag, "work");
    head.cmd = 1;
    head.len = in.size();

    char send_buff[send_buff_size];
    std::memcpy(send_buff, (char *)&head, sizeof(MsgHead));
    std::memcpy(send_buff + sizeof(MsgHead), in.data(), in.size());
    
    int len = sizeof(MsgHead) + in.size();
    m_socket.send(send_buff, len);

    // char recv_buff[recv_buff_size];
    // int recv_size = m_socket.recv(recv_buff, recv_buff_size);
    
    // DataStream out;
    // out.write(recv_buff, recv_size);
}

}
}