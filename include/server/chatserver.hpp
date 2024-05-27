#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
using namespace muduo;
using namespace std;
using namespace muduo::net;

class ChatServer{
public:
    // 初始化聊天服务器对象
    ChatServer(EventLoop* loop, const InetAddress& listenAddr, const string& nameArg);
    // 启动服务器
    void start();

private:
    // 连接相关信息的回调函数，如果有连接就会调用这个函数
    void onConnection(const TcpConnectionPtr&);
    // 读写相关信息的回调函数，如果有读写就会调用这个函数
    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

    // muduo库的类对象，实现服务器功能的
    TcpServer _server;
    // 指向事件循环对象的指针
    EventLoop *_loop;

};

#endif