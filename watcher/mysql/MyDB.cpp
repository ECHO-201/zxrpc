#include <iostream>
#include <string>
#include "MyDB.h"
#include "config_file.h"

using namespace watcher::tools;
using namespace watcher::mysql;  

MyDB::MyDB()
{
    mysql = mysql_init(NULL);   
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
    if(mysql!=NULL)  
    {
        mysql_close(mysql);
    }
}


bool MyDB::initDB(std::string host,std::string user,std::string passwd,std::string db_name)
{ 
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
    if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<<"Query Error: "<<mysql_error(mysql);
        return false;
    }
    else 
    {
        result = mysql_store_result(mysql); 
        if (result)  
        {
           int  num_fields = mysql_num_fields(result);   
           int  num_rows=mysql_num_rows(result);       
           for(int i=0;i<num_rows;i++) 
            {
                row=mysql_fetch_row(result);
                if(row<0) break;
                for(int j=0;j<num_fields;j++)  
                {
                    std::cout<<row[j]<<"\t\t";
                }
                std::cout<<std::endl;
            }
        }
        else  
        {
            if(mysql_field_count(mysql) == 0)   
            {
                
                int num_rows = mysql_affected_rows(mysql); 
            }
            else 
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
    else 
    {
        result = mysql_store_result(mysql); 
        if (result)  
        { 
           int  num_fields = mysql_num_fields(result);   
           int  num_rows = mysql_num_rows(result);       
           for(int i = 0; i < num_rows; ++i) 
            {
                
                row=mysql_fetch_row(result);
                if(row < 0) break;
                user dt;
                dt.id = "";
                dt.name = "";
                dt.passwd = "";
                dt.id = row[0];
                dt.name = row[1];
                dt.passwd = row[2];
                
                cour.push_back(dt);
               
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
    else 
    {
        result = mysql_store_result(mysql);  
        if (result)  
        { 
           int  num_fields = mysql_num_fields(result);   
           int  num_rows = mysql_num_rows(result);       
           for(int i=0; i<num_rows; ++i) 
            {
                
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
                
                cour_log.push_back(dt);
                
            }
        }
        return cour_log;    
    }
};

std::vector<user> MyDB::get_user(std::string sql){
   if (mysql_query(mysql,sql.c_str()))
    {
        std::cout<< "Query Error: " << mysql_error(mysql);
        return cour;
    }
    else 
    {
        result = mysql_store_result(mysql);  
        if (result)  
        { 
           int  num_fields = mysql_num_fields(result);  
           int  num_rows = mysql_num_rows(result);       
           for(int i=0; i<num_rows; ++i) 
            {
                
                row = mysql_fetch_row(result);
                if(row < 0) break;
                user dt;
                dt.name = row[0];
                dt.phone = row[1];
                cour.push_back(dt);
            }
        }
        return cour;    
    }
};

bool MyDB::exeSQLinsert(std::string sql){   
    bool flag; 
    if (mysql_query(mysql, sql.c_str()))        
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
    else 
    {
        result = mysql_store_result(mysql);  
        if (result)  
        { 
           int  num_fields = mysql_num_fields(result);  
           int  num_rows = mysql_num_rows(result);     
           
               
                row = mysql_fetch_row(result);
                if(row < 0) return "NULL";
        }
        return row[0];    
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