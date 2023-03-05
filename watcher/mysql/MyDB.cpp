#include <iostream>
#include <string>
#include "MyDB.h"
#include "config_file.h"

using namespace watcher::tools;
using namespace watcher::mysql;  

MyDB::MyDB()
{
    mysql = mysql_init(NULL);   //初始化数据库连接变量
    if(mysql == NULL)
    {
        std::cout << "Error:" << mysql_error(mysql);
        exit(1);
    }
    std::string db_host = config_file::instance()->GetStringInfo("mysql","db_host","192.168.16.4");
    std::string db_user = config_file::instance()->GetStringInfo("mysql","db_user","root");
    std::string db_passwd = config_file::instance()->GetStringInfo("mysql","db_passwd","123456");
    std::string db_table = config_file::instance()->GetStringInfo("mysql","db_table","android");
	bool flag = initDB(db_host, db_user, db_passwd, db_table);
    if(!flag){
        std::cout<< "mysql connect fail" << std::endl;
    }
}

MyDB::~MyDB()
{
    if(mysql!=NULL)  //关闭数据连接
    {
        mysql_close(mysql);
    }
}


bool MyDB::initDB(std::string host,std::string user,std::string passwd,std::string db_name)
{
    // 函数mysql_real_connect建立一个数据库连接  
    // 成功返回MYSQL*连接句柄，失败返回NULL  
    mysql = mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str(), db_name.c_str(), 0, NULL, 0);
    mysql_set_character_set(mysql, "utf8");  
    if(mysql == NULL)  
    {  
        std::cout << "Error: " << mysql_error(mysql);  
        exit(1);  
    }  
    return true;  
}

bool MyDB::exeSQL(std::string sql)
{
    //(result);
    //mysql_query()执行成功返回0,执行失败返回非0值。
    if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<<"Query Error: "<<mysql_error(mysql);
        return false;
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
                for(int j=0;j<num_fields;j++)  //输出每一字段
                {
                    std::cout<<row[j]<<"\t\t";
                }
                std::cout<<std::endl;
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
                std::cout<<"Get result error: "<<mysql_error(mysql);
                return false;
            }
        }
        mysql_free_result(result);
    }


    return true;

}

std::vector<user> MyDB::exe_select(std::string sql){
   if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<<"Query Error: "<<mysql_error(mysql);
        return cour;
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        { 
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows = mysql_num_rows(result);       //获取结果集中总共的行数
           for(int i = 0; i < num_rows; ++i) //输出每一行
            {
                //获取下一行数据
                row=mysql_fetch_row(result);
                if(row < 0) break;
                user dt;
                dt.id = "";
                dt.name = "";
                dt.passwd = "";
                dt.id = row[0];
                dt.name = row[1];
                dt.passwd = row[2];
                /*for(int j=0;j<num_fields;++j)  //输出每一字段
                {
                    std::cout<<row[j]<<"\t\t";
                }*/
                cour.push_back(dt);
                //std::cout<<std::endl;
            }
        }
        return cour;    
    }
};

std::vector<log_data> MyDB::get_log(std::string sql){
   if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<< "Query Error: " << mysql_error(mysql);
        return cour_log;
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        { 
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows = mysql_num_rows(result);       //获取结果集中总共的行数
           for(int i=0; i<num_rows; ++i) //输出每一行
            {
                //获取下一行数据
                row = mysql_fetch_row(result);
                if(row < 0) break;
                log_data dt;
                dt.id = "";
                dt.ip = "";
                dt.port = "";
                dt.record = "";
                dt.id = row[0];
                dt.ip = row[1];
                dt.port = row[2];
                dt.record = row[3];
                /*for(int j=0;j<num_fields;++j)  //输出每一字段
                {
                    std::cout<<row[j]<<"\t\t";
                }*/
                cour_log.push_back(dt);
                //std::cout<<std::endl;
            }
        }
        return cour_log;    
    }
};

bool MyDB::exeSQLinsert(std::string sql){   
    bool flag; 
    if (mysql_query(mysql, sql.c_str()))        //执行SQL语句  
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
   if (mysql_query(mysql, sql.c_str()))
    {
        std::cout << "Query Error: " << mysql_error(mysql);
        return "NULL";
    }
    else // 查询成功
    {
        result = mysql_store_result(mysql);  //获取结果集
        if (result)  // 返回了结果集
        { 
           int  num_fields = mysql_num_fields(result);   //获取结果集中总共的字段数，即列数
           int  num_rows = mysql_num_rows(result);       //获取结果集中总共的行数
           
                //获取下一行数据
                row = mysql_fetch_row(result);
                if(row < 0) return "NULL";
        }
        return row[0];    
    }
};

std::string MyDB::exeSQLInfor(std::string sql){
    if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<<"Query Error: "<<mysql_error(mysql);
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
                row = mysql_fetch_row(result);
                if(row<0) return "NULL";
        }
        std::string s=row[0];std::string st=row[1];
        return s+":"+st;    
    }
};

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