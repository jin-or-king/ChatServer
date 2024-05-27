#include "chatserver.hpp"
#include <functional>
#include "json.hpp"
#include <string>
#include "chatservice.hpp"
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

// 网络模块代码，执行服务器的连接释放
ChatServer::ChatServer(EventLoop *loop, // 事件循环
               const InetAddress &listenAddr, // IP+Port
               const string &nameArg) // 服务器的名字
        : _server(loop, listenAddr, nameArg), _loop(loop) {

    // 给服务器注册用户连接的创建和断开回调
    _server.setConnectionCallback(bind(&ChatServer::onConnection, this, _1));
    //给服务器注册用户读写事件的回调
    _server.setMessageCallback(bind(&ChatServer::onMessage, this, _1, _2, _3));
    //设置服务器端的线程数量 1个IO线程（监听用户的连接），3个worker线程
    _server.setThreadNum(4);
}

// 开启事件循环
void ChatServer::start(){
    _server.start();
}

// 专门处理用户连接和创建断开 直接使用这个函数，底层实现从epoll取得一个listenfd进行accept
void ChatServer::onConnection(const TcpConnectionPtr& conn){
    if(!conn->connected()){
        // 断开连接前有相应的业务逻辑要完成
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 专门处理用户读写事件
void ChatServer::onMessage(const TcpConnectionPtr &conn, // 连接
                Buffer *buf, // 缓冲区
                Timestamp time){ // 接受数据的时间信息
    string buffer = buf->retrieveAllAsString();

    json js = json::parse(buffer);

    // 解耦网络模块和业务模块。通过js["msgid"]获取->业务handler->填入参数conn js time
    auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
    msgHandler(conn, js, time);
}