#include <iostream>
#include<string>
// #include "MySQLConnect/ConnectionPool.h"
#include "MysqlCon.h"

void inquery();

int main(int argc, char** argv)
{
  inquery();
}

void inquery()
{
    MysqlCon conn;
    bool flag = conn.connect("root","","testdb","localhost", 3306);
    if(!flag)
    {
        std::cout<<"Error connect"<<std::endl;
        exit(1);
    }

    std::string sql_test{"insert into index_demo values(25,2021,'m')"};
    flag = conn.update(sql_test);
    if(!flag)
    {
        std::cout<<"Error insert:{"<<sql_test<<"}"<<std::endl;
        exit(1);
    }
    std::string query_test{"select* from index_demo"};
    conn.query(query_test);
    while(conn.next())
    {
        std::cout<<conn.getValue(0)<<","<<conn.getValue(1)<<","<<conn.getValue(2)<<std::endl;
    }
}
