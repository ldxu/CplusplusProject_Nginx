#ifndef _CLIENT_H_
#define _CLIENT_H__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include "global.h"
#include "log.h"
#include "crc32.h"
#include "cli_config.h"

class Client
    {
        private:
                //套接字句柄

                int sockfd=-1;
                // 用来保存服务器的地址信息
                struct sockaddr_in  SIN;
                //都是单例类 不需要主动释放
                ConfigManager*      Cfg;
                CCRC32*             Crc32;
                DailyLogger*        Logger;
                
        public:
            // 与服务器建立连接
            void GetSocket(int __domain = AF_INET, int __type = SOCK_STREAM, int __protocol = 0);
            void GetConnect();
            void MsgSendThreadFunc(){};
            void MsgRecvThreadFunc(){};
        public:
            Client(const string& configName = "../config.cfg");
            ~Client();
    };
#endif