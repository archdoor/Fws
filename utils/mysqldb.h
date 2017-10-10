#ifndef __MYSQLDB_H__
#define __MYSQLDB_H__

#include <mysql/mysql.h>

class MysqlDb
{
public:
    MysqlDb();
    int connect(const char *ip, const char *username, const char *pwd, const char *db); 
    int query(const char *query);
    void print_query_result();
    unsigned long get_affected_rows();
    void close();


    MYSQL *m_conn;
};


#endif
