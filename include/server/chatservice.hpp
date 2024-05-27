#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/base/Timestamp.h>
#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include "/home/huangjin/myChat/thirdparty/json.hpp"
#include <memory>
#include "/home/huangjin/myChat/include/server/model/usermodel.hpp"
#include "/home/huangjin/myChat/include/server/model/offlinemessagemodel.hpp"
#include <mutex>
#include "/home/huangjin/myChat/include/server/model/friendmodel.hpp"
#include "/home/huangjin/myChat/include/server/model/groupmodel.hpp"
#include "/home/huangjin/myChat/include/server/redis/redis.hpp"

using json = nlohmann::json;

using namespace std;
using namespace muduo;
using namespace muduo::net;

// 处理消息的事件回调方法类型，用来存放login、reg等业务流程函数
using MsgHandler = function<void(const TcpConnectionPtr& conn, json& js, Timestamp )>;

// 聊天服务器业务类，单例
class ChatService{
public:
    // 处理登录业务
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 处理注册业务
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 获取单例模式的接口
    static ChatService* instance();

    // 获取消息对应的处理函数
    MsgHandler getHandler(int msgid);
        
    // 处理用户异常退出
    void clientCloseException(const TcpConnectionPtr& conn);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 服务器异常，业务重置方法
    void reset();

    // 添加好友业务
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr& conn, json &js, Timestamp time);
    
    // 加入群组业务
    void addGroup(const TcpConnectionPtr& conn, json &js, Timestamp time);
    
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int userid, string msg);
private:
    // 完成单例模式，将构造函数私有化，暴露一个static接口
    ChatService();

    // 消息存储id和其对应的业务处理方法，收到不同的id，进入不同的业务流程
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 存储不同用户id的连接conn
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 用户添加进入_userConnMap需要锁
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;

    // 离线表的操控类对象
    OfflineMsgModel _offlineMsgModel;

    // 好友表的操纵类对象
    FriendModel _friendModel;
    
    // 群组表的操纵类对象
    GroupModel _groupModel;

    // redis操作对象
    Redis _redis;
};

#endif