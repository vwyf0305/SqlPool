//
// Created by wuyifei0305 on 22-10-21.
//

#include "ConnectionPool.h"

ConnectionPool* ConnectionPool::getConnectPool() {
    static ConnectionPool pool;
    return &pool;
}

bool ConnectionPool::ParseJSONfiles() {
    std::fstream fjson{"configs/dbconf.json"};
    Json::Reader json_readr;
    Json::Value root;
    json_readr.parse(fjson, root);
    if(root.isObject())
    {
        m_ip = root["ip"].asString();
        m_port = root["port"].asInt();
        m_dbName = root["dbName"].asString();
        m_user = root["userName"].asString();
        m_passward = root["passward"].asString();
        m_timeout = root["timeout"].asInt();
        m_maxIdleTime = root["maxIdelTime"].asInt();
        m_maxSize = root["maxSize"].asInt();
        m_minSize = root["minSize"].asInt();
        return true;
    }
    else
    {
        return false;
    }
}

ConnectionPool::ConnectionPool() {
    if(ParseJSONfiles())
    {
        std::cout<<"Failed to parse json files.\n";
        exit(1);
    }
    for(int i=0;i!=m_minSize;i++)
    {
         addConnection();
    }
    std::thread producer(&ConnectionPool::produceConnection, this);
    producer.detach();
    std::thread recycler(&ConnectionPool::recycleConnection, this);
    recycler.detach();
}

void ConnectionPool::produceConnection() {
    while(true)
    {
        std::unique_lock<std::mutex> locker(m_mutexQ);
        while(m_connectionQ.size() >= m_minSize)
        {
            m_cond.wait(locker);
        }
         addConnection();
        m_cond.notify_all();
    }
}

void ConnectionPool::recycleConnection() {
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        while (m_connectionQ.size() > m_maxSize)
        {
            auto front_conn = m_connectionQ.front();
            if(front_conn->getAliveTime() >= m_maxIdleTime)
            {
                m_connectionQ.pop();
                delete front_conn;
            }
            else
                break;
        }
    }
}

void ConnectionPool::addConnection() {
    MysqlCon* conn{new MysqlCon};
    conn->connect(m_user, m_passward, m_dbName, m_ip, m_port);
    conn->refreshAliveTime();
    m_connectionQ.push(conn);
}

std::shared_ptr<MysqlCon> ConnectionPool::getConnection() {
    std::unique_lock<std::mutex> locker(m_mutexQ);
    while(m_connectionQ.empty())
    {
        if(std::cv_status::timeout == m_cond.wait_for(locker,std::chrono::milliseconds(m_timeout)))
        {
            if(m_connectionQ.empty())
            {
                continue;
            }
        }
    }
    auto deleter = [this](MysqlCon* conn){
        std::lock_guard<std::mutex> guard(m_mutexQ);
        conn->refreshAliveTime();
        m_connectionQ.push(conn);
    };
    std::shared_ptr<MysqlCon> conn(m_connectionQ.front(), deleter);
    m_connectionQ.pop();
    m_cond.notify_all();
    return conn;
}

ConnectionPool::~ConnectionPool() {
    while(!m_connectionQ.empty())
    {
        auto conn = m_connectionQ.front();
        m_connectionQ.pop();
        delete conn;
    }
}