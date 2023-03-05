#ifndef _MYDB_H
#define _MYDB_H
#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <vector>
#include "struct.h"
//using namespace std;

namespace watcher {
namespace mysql {

class MyDB
{
public:
    MyDB();
    ~MyDB();
    bool initDB(std::string host,std::string user,std::string pwd,std::string db_name); //连接mysql
    bool exeSQL(std::string sql);   //执行sql语句
    std::vector<user> exe_select(std::string sql);
    bool exeSQLinsert(std::string sql);
    std::string exeSQLdesc(std::string sql);
    std::string exeSQLInfor(std::string sql);
    std::vector<log_data> get_log(std::string sql);
    bool begin_transaction();
    bool commit_transaction();
    bool rollback_transaction();
    
private:
    MYSQL *mysql;          //连接mysql句柄指针
    MYSQL_RES *result;    //指向查询结果的指针
    MYSQL_ROW row;       //按行返回的查询信息
    
    std::vector<user> cour;
    std::vector<log_data> cour_log;
};

}}

#endif
