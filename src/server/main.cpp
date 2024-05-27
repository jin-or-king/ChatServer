#include "chatserver.hpp"
#include "chatservice.hpp"
#include <signal.h>
#include <iostream>

// 处理服务器ctrl+c结束后，重置user的状态信息
void resetHandler(int)
{
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv){
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatServer 222.204.61.156 6000" << endl;
        exit(-1);
    }

    // 解析通过命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    EventLoop loop; // epoll

    InetAddress addr(ip, port);

    ChatServer server(&loop, addr, "ChatServer");

    // 捕捉ctrl+c信号并执行自定义的处理函数
    signal(SIGINT, resetHandler);

    //启动服务，将listenfd epoll_ctl->epoll
    server.start();
    //epoll_wait以阻塞的方式等待新用户的连接
    loop.loop();
    return 0;
}