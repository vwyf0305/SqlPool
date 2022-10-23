//
// Created by wuyifei0305 on 22-10-21.
//
#include "MysqlCon.h"
#include<iostream>
#include <chrono>
#include<queue>
#include<thread>
#include<mutex>
#include<memory>
#include<condition_variable>
#include<json/json.h>
#include<fstream>
#pragma once
// 懒汉单例模式

class ConnectionPool {
public:
    static ConnectionPool* getConnectPool();
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool(const ConnectionPool&&) = delete;
    ConnectionPool& operator= (const ConnectionPool&) = delete;
    ~ConnectionPool();
    // MysqlCon* getConnection();
    std::shared_ptr<MysqlCon> getConnection();
private:
    ConnectionPool();
    bool ParseJSONfiles();
    void produceConnection();
    void recycleConnection();
    void addConnection();
    std::string m_user{"root"};
    std::string m_passward{"123456"};
    std::string m_ip;
    std::string m_dbName;
    ushort m_port;
    int m_minSize;
    int m_maxSize;
    int m_timeout;
    int m_maxIdleTime;
    std::mutex m_mutexQ;
    std::condition_variable m_cond;
    std::queue<MysqlCon*> m_connectionQ;
};



