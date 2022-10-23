//
// Created by wuyifei0305 on 22-10-21.
//

#include "MysqlCon.h"

MysqlCon::MysqlCon() {
    m_conn = mysql_init(nullptr);
    mysql_set_character_set(m_conn, "utf8");
}

MysqlCon::~MysqlCon() {
    freeResult();
    if(m_conn != nullptr)
    {
        mysql_close(m_conn);
    }
}

bool MysqlCon::connect(std::string user, std::string passward, std::string dbName, std::string ip, ushort port) {
    MYSQL * ptr = mysql_real_connect(m_conn, ip.c_str(), user.c_str(), passward.c_str(), dbName.c_str(), port, nullptr, 0);
    return ptr != nullptr;
}

bool MysqlCon::update(std::string sql) {
    int res {mysql_query(m_conn, sql.c_str())};
    return res == 0;
}

bool MysqlCon::query(std::string sql) {
    freeResult();
    int res {mysql_query(m_conn, sql.c_str())};
    if(res == 0)
    {
        m_result = mysql_store_result(m_conn);
        return true;
    }
    else
    {
        return false;
    }
}
void MysqlCon::getRowCount() {
    rowCount = mysql_num_fields(m_result);
}
bool MysqlCon::next() {
    if(m_result != nullptr)
    {
         m_row = mysql_fetch_row(m_result);
    }
    return true;
}

std::string MysqlCon::getValue(int index) {
    MysqlCon::getRowCount();
    if(index > rowCount || index <0)
    {
        return "error";
    }
    std::string value{m_row[index]};
    return value;
}

inline bool MysqlCon::transaction()  {
    return mysql_autocommit(m_conn, false);
}

inline bool MysqlCon::commit() {
    return mysql_commit(m_conn);
}

inline bool MysqlCon::rollback() {
    return mysql_rollback(m_conn);
}

void MysqlCon::freeResult() {
    if(m_result != nullptr)
    {
        mysql_free_result(m_result);
        m_result = nullptr;
    }
}

void MysqlCon::refreshAliveTime() {
    m_aliveTime = std::chrono::steady_clock::now();
}

long long MysqlCon::getAliveTime() {
    std::chrono::nanoseconds res {std::chrono::steady_clock::now() - m_aliveTime};
    std::chrono::milliseconds mill_res = std::chrono::duration_cast<std::chrono::milliseconds>(res);
    return mill_res.count();
}