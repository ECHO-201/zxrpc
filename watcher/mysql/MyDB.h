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
    bool initDB(std::string host,std::string user,std::string pwd,std::string db_name);
    bool exeSQL(std::string sql);   
    std::vector<user> exe_select(std::string sql);
    bool exeSQLinsert(std::string sql);
    std::string exeSQLdesc(std::string sql);
    std::vector<log_data> get_log(std::string sql);
    std::vector<user> get_user(std::string sql);
    bool begin_transaction();
    bool commit_transaction();
    bool rollback_transaction();
    
private:
    MYSQL *mysql;        
    MYSQL_RES *result;   
    MYSQL_ROW row;     
    
    std::vector<user> cour;
    std::vector<log_data> cour_log;
};

}}

#endif
