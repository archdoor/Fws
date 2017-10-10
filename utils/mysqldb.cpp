#include <stdio.h>
#include "mysqldb.h"

MysqlDb::MysqlDb()
{
    m_conn = mysql_init(NULL);
    if ( !m_conn )
    {
        m_conn = NULL;
    }
}

// 连接数据库
int MysqlDb::connect(const char *ip, const char *username, const char *pwd, const char *db)
{
    if ( !mysql_real_connect( m_conn, ip, username, pwd, db, 0, NULL, 0  )  )
    {
        return -1;
    }

    return 0;
}

// 数据库查询
int MysqlDb::query(const char *query)
{
    if ( mysql_query(m_conn, query) != 0 )
    {
        return -1;
    }

    return 0;
}

// 获取改变行数
unsigned long MysqlDb::get_affected_rows()
{
    unsigned long num_rows = (unsigned long)mysql_affected_rows(m_conn);
    return num_rows;
}

// 打印查询结果(调试使用)
void MysqlDb::print_query_result()
{
    MYSQL_RES *sqlres;
    MYSQL_ROW sqlrow;

    // 获取全部结果
    sqlres = mysql_store_result(m_conn);
    if(sqlres)  
    {  
        int num_rows = mysql_num_rows(sqlres);
        int num_fields = mysql_num_fields(sqlres);
        printf("query result %d rows, each with %d field.\n", num_rows, num_fields);  
        while( (sqlrow = mysql_fetch_row(sqlres)) )  
        {  
            for ( int i = 0; i < num_fields; ++i )
                printf("%s\t", sqlrow[i]);  
            printf("\n");  
        }  

        mysql_free_result(sqlres);  
    }  
}

// 关闭数据库连接
void MysqlDb::close()
{
    mysql_close(m_conn);
}

