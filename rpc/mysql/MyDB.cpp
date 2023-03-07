#include "MyDB.h"
#include "config_file.h"

using namespace zxrpc::mysql;
using namespace zxrpc::tools;

MyDB::MyDB()
{
    mysql = mysql_init(NULL);   //初始化数据库连接变量
    if(mysql == NULL)
    {
        std::cout<< "Error: " << mysql_error(mysql) << std::endl; 
        exit(1);
    }
}

MyDB::~MyDB()
{
    if(mysql != NULL)  //关闭数据连接
    {
        mysql_close(mysql);
    }
}

bool MyDB::connect(){
    m_ip = config_file::instance()->GetStringInfo("mysql", "mysqlip", "0.0.0.0");
    m_port = config_file::instance()->GetNumberInfo("mysql", "mysqlport", 3306);
    m_user = config_file::instance()->GetStringInfo("mysql", "mysqluser", "root");
    m_passwd = config_file::instance()->GetStringInfo("mysql", "mysqlpasswd", "123456");
    m_db_name = config_file::instance()->GetStringInfo("mysql", "mysqltable", "rpc");
    //std::cout << m_ip << " "<< m_user << " " << m_passwd << " " << m_db_name <<" "<< m_port << std::endl;
    return initDB(m_ip, m_port, m_user, m_passwd, m_db_name);
}

bool MyDB::initDB(std::string &host, int port, std::string &user,std::string &passwd,std::string &db_name)
{
    // 函数mysql_real_connect建立一个数据库连接  
    // 成功返回MYSQL*连接句柄，失败返回NULL 
    mysql_set_character_set(mysql, "utf8");
    mysql = mysql_real_connect(mysql, host.c_str(), 
        user.c_str(), 
        passwd.c_str(), 
        db_name.c_str(), 0, NULL, 0);  
    if(mysql == NULL)  
    {  
        std::cout<< "Error: " <<  mysql_error(mysql) << std::endl;
        exit(1); 
    }
    
    return true;  
}

bool MyDB::exeSQL(std::string sql)
{
    //(result);
    //mysql_query()执行成功返回0,执行失败返回非0值
    if (mysql_query(mysql, sql.c_str()))
    {
        std::cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        {
           int num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int num_rows = mysql_num_rows(result);       //获取结果集中总共的行数
           for(int i = 0; i < num_rows; ++i) //输出每一行
            {
                //获取下一行数据
                row = mysql_fetch_row(result);
                if(row<0) break;
                // for(int j = 0; j < num_fields; ++j)  //输出每一字段
                // {
                //     std::cout<< row[j] << "\t\t";
                // }
                // std::cout << std::endl;
            }
        }
        else  // result==NULL
        {
            if(mysql_field_count(mysql) == 0)   //代表执行的是update,insert,delete类的非查询语句
            {
                // (it was not a SELECT)
                
                int num_rows = mysql_affected_rows(mysql);  //返回update,insert,delete影响的行数
            }
            else // error
            {
                std::cout<<"Get result error: "<< mysql_error(mysql);
                return false;
            }
        }
        mysql_free_result(result);
    }

    return true;
}

std::vector<user> MyDB::exeSQLs(std::string sql){
   if (mysql_query(mysql,sql.c_str()))
    {
        std::cout << "Query Error: " << mysql_error(mysql);
        return cour;
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        { 
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows=mysql_num_rows(result);       //获取结果集中总共的行数
           for(int i=0;i<num_rows;i++) //输出每一行
            {
                //获取下一行数据
                row=mysql_fetch_row(result);
                if(row<0) break;
                user ce;
                ce.id = "";
                ce.name = "";
                ce.passwd = "";
                ce.id = row[0];
                ce.name = row[1];
                ce.passwd = row[2];
                /*for(int j=0;j<num_fields;++j)  //输出每一字段
                {
                    std::cout<<row[j]<<"\t\t";
                }*/
                cour.push_back(ce);
                //std::cout<<std::endl;
            }
        }
        return cour;    
    }
};

bool MyDB::exeSQLinsert(std::string sql){   
    bool flag; 
    if (mysql_query(mysql,sql.c_str()))        //执行SQL语句  
	{
		printf("Query failed (%s)\n", mysql_error(mysql));
		flag = false;
	}
	else
	{
		printf("Insert success\n");
		flag = true;
	}
    return flag;
}

std::string MyDB::exeSQLdesc(std::string sql){
   if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<< "Query Error: " << mysql_error(mysql);
        return "NULL";
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        { 
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows=mysql_num_rows(result);       //获取结果集中总共的行数
           
                //获取下一行数据
                row=mysql_fetch_row(result);
                if(row<0) return "NULL";
        }
        return row[0];    
    }
};

bool MyDB::select_SQL(std::string sql)
{
    //(result);
    //mysql_query()执行成功返回0,执行失败返回非0值
    if (mysql_query(mysql, sql.c_str()))
    {
        std::cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        {
           int num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int num_rows = mysql_num_rows(result);       //获取结果集中总共的行数
           return num_rows;
        }
        else  // result==NULL
        {
            if(mysql_field_count(mysql) == 0)   //代表执行的是update,insert,delete类的非查询语句
            {
                // (it was not a SELECT)
                
                int num_rows = mysql_affected_rows(mysql);  //返回update,insert,delete影响的行数
            }
            else // error
            {
                std::cout<<"Get result error: "<<mysql_error(mysql);
                return false;
            }
        }
        mysql_free_result(result);
    }

    return true;
}

bool MyDB::begin_transaction() {
    if (mysql_query(mysql, "BEGIN")) {
        std::cout << "begin error" << std::endl;
        return false;
    }
    return true;
}

bool MyDB::commit_transaction(){
    if (mysql_query(mysql, "COMMIT")) {
        std::cout << "rollback error" << std::endl;
        return false;
    }
    return true;
}

bool MyDB::rollback_transaction(){
    if (mysql_query(mysql, "ROLLBACK")) {
        std::cout << "rollback error" << std::endl;
        return false;
    }
    return true;
}