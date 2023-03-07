#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>

const int buf_size = 1024;

struct MsgHead{
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};


// int main()
// {
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd == -1)
//     {
//         std::cout << "socket is error" << std::endl;
//         exit(1);
//     }
//     struct sockaddr_in servaddr;
//     memset(&servaddr, 0, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     servaddr.sin_port = htons(10001);
//     int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
//     if (ret == -1)
//     {
//         std::cout <<"connect is error" << std::endl;
//         exit(1);
//     }
//     char buf[buf_size];
//     memset(buf, 0, sizeof(buf));
//     char msg[] = "Hello World!";
//     MsgHead msghead = {"work", 1, sizeof(msg)};

//     //等价于
//     // memset(&msghead, 0, sizeof(msghead));
//     // memcpy(msghead.flag, "work", 4);
//     // msghead.cmd = 1;
//     // msghead.len = strlen(msg);

//     memcpy(buf, (char*)&msghead, sizeof(msghead));
//     memcpy(buf + sizeof(msghead), msg, sizeof(msg));
//     ret = send(sockfd, buf, sizeof(msghead) + sizeof(msg), 0);
//     std::cout << "send bytes: " << ret << std::endl;
//     memset(buf, 0, sizeof(buf));
//     ret = recv(sockfd, buf, buf_size, 0);
//     std::cout << "recv bytes is " << ret << ", " << buf << std::endl;

//     close(sockfd);
//     return 0;
// }
