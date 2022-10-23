#include <iostream>
#include<string>
#include<memory>
#include<chrono>
#include<fmt/core.h>
#include "ConnectionPool.h"
#include "MysqlCon.h"

// void inquery();
void op(ConnectionPool* pool, int begin, int end);
void test_pool_time();
using PointTime = std::chrono::steady_clock::time_point;
int main(int argc, char** argv)
{
    test_pool_time();
}

/*
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
*/
void op(ConnectionPool* pool, int begin, int end)
{
    for(int i=begin;i!=end;i++)
    {
        auto conn = pool->getConnection();
        std::string sql_test =  fmt::format("insert into index_demo values({},2021,'m')", std::to_string(i));
        conn->update(sql_test);
    }
}

void test_pool_time()
{
    auto pool = ConnectionPool::getConnectPool();
    PointTime begin = std::chrono::steady_clock::now();
    PointTime end = std::chrono::steady_clock::now();
    op(pool,40,200);
    auto length = end-begin;
    std::string length_str = std::to_string(length.count());
    fmt::print("The duration of sqlpool is {} nanoseconds.\n", length_str);
    length_str = std::to_string(length.count()/1000000);
    fmt::print("The duration of sqlpool is {} milliseconds.\n", length_str);
}

