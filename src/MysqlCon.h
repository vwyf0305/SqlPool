#include<string>
#include<memory>
#include<chrono>
#include<mysql/mysql.h>

using ushort = unsigned  short;
#pragma once
class MysqlCon
{
public:
    MysqlCon();
    ~MysqlCon();
    bool connect(std::string user, std::string passward, std::string dbName, std::string ip, ushort port=3306); // 连接数据库
    bool update(std::string sql);
    bool query(std::string sql);
    bool next();
    std::string getValue(int index);
    void refreshAliveTime();
    long long getAliveTime();
    inline bool transaction();
    inline bool commit();
    inline bool rollback();
private:
    MYSQL* m_conn{nullptr};
    MYSQL_RES* m_result{nullptr};
    MYSQL_ROW m_row{nullptr};
    std::chrono::steady_clock::time_point m_aliveTime;
    int rowCount;
    inline void getRowCount();
    void freeResult();
 };
