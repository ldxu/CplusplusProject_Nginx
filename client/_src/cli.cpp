#include <iostream>
#include <cstring>
#include <thread>
#include "client.h"
#define MAX 100

const string ConfigName = "../config.cfg";

int main(int argc,char *argv[])
{
    Client cli(ConfigName);
    // 创建套接字
    cli.GetSocket();
    //与服务器连接
    cli.GetConnect();
    //走到这一步说明已经连接上了 准备创建读线程和写线程
    return 0;
}        