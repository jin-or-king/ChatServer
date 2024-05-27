#ifndef USERMODEL_H
#define USERMODEL_H

#include "/home/huangjin/myChat/include/server/model/user.hpp"

// User表的数据操作类
class UserModel{
public:
    // User表的增加方法
    bool insert(User& user);

    // 根据用户id查询信息
    User query(int id);

    // 登录后跟新用户状态
    bool updateState(User& user);

    // 重置用户的状态信息
    void resetState();

private:
};


#endif