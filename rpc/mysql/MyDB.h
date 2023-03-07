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
    bool exeSQL(std::string sql);  
    std::vector<user> exeSQLs(std::string sql);
    bool exeSQLinsert(std::string sql);
    std::string exeSQLdesc(std::string sql);
    bool select_SQL(std::string sql);
    
    bool begin_transaction();
    bool commit_transaction();
    bool rollback_transaction();
private:
    MYSQL *mysql;          
    MYSQL_RES *result;    
    MYSQL_ROW row;       

    std::string m_ip;
    int m_port;
    std::string m_db_name;
    std::string m_user;
    std::string m_passwd;
    std::vector<user> cour;
};

}}


