#pragma once

#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <vector>
#include "struct.h"

//using namespace std;

namespace zxrpc{
namespace mysql{

class MyDB
{
public:
    MyDB();
    ~MyDB();
    bool connect();
    bool initDB(std::string &host, int port, std::string &user,std::string &pwd,std::string &db_name); //连接mysql
    bool exeSQL(std::string sql);   //执行sql语句
    std::vector<user> exeSQLs(std::string sql);
    bool exeSQLinsert(std::string sql);
    std::string exeSQLdesc(std::string sql);
    bool select_SQL(std::string sql);
    
    bool begin_transaction();
    bool commit_transaction();
    bool rollback_transaction();
private:
    MYSQL *mysql;          //连接mysql句柄指针
    MYSQL_RES *result;    //指向查询结果的指针
    MYSQL_ROW row;       //按行返回的查询信息

    std::string m_ip;
    int m_port;
    std::string m_db_name;
    std::string m_user;
    std::string m_passwd;
    std::vector<user> cour;
};

}}


