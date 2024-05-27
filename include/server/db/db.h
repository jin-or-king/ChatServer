#ifndef DB_H
#define DB_H

#include <mysql/mysql.h>
#include <string>
using namespace std;


// 数据库操作类
class MySQL
{
public:

    // 初始化数据库连接，让系统分配一个存放连接的内存地址
    MySQL();

    // 释放数据库连接资源，释放内存资源
    ~MySQL();

    // 连接数据库，进行实际的数据库连接（ip地址端口，用户名密码，数据库名）
    bool connect();

    // 更新操作     
    bool update(string sql);

    // 查询操作
    MYSQL_RES* query(string sql);

    MYSQL* getConnection();
private:
    MYSQL *_conn;
};



#endif