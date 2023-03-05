#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>

using namespace std;

#define BUFSIZE 1024

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
//         perror("socket()");
//         exit(1);
//     }
//     struct sockaddr_in servaddr;
//     memset(&servaddr, 0, sizeof servaddr);
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     servaddr.sin_port = htons(10006);
//     int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof servaddr);
//     if (ret == -1)
//     {
//         perror("connect()");
//         exit(1);
//     }
//     char buf[BUFSIZE];
//     memset(buf, 0, sizeof buf);
//     char msg[] = "Hello World!";
//     MsgHead msghead = {"work", 1, sizeof msg};
//     // memset(&msghead, 0, sizeof msghead);
//     // memcpy(msghead.flag, "work", 4);
//     // msghead.cmd = 1;
//     // msghead.len = strlen(msg);    
//     memcpy(buf, (char*)&msghead, sizeof msghead);
//     cout << buf << endl;
//     memcpy(buf + sizeof msghead, msg, sizeof msg);
//     cout << buf << endl;
//     ret = send(sockfd, buf, sizeof msghead + sizeof msg, 0);
//     cout << "send bytes: " << ret << endl;
//     memset(buf, 0, sizeof buf);
//     ret = recv(sockfd, buf, BUFSIZE, 0);
//     cout << "recv bytes: " << ret << ", " << buf << endl;

//     close(sockfd);
//     return 0;
// }
