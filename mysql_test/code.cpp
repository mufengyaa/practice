#include <iostream>
#include <string>
#include <mysql/mysql.h>

void client(MYSQL *mysql)
{
    std::string sql;
    std::cout << "mysql>";
    std::cout.flush();
    while (std::getline(std::cin, sql))
    {
        if (sql == "quit")
        {
            std::cout << "bye\n";
            break;
        }
        int ret = mysql_query(mysql, sql.c_str());
        if (ret == 0)
        {
            std::cout << sql << " success\n";
        }
        else
        {
            std::cout << mysql_error(mysql) << std::endl;
        }
        std::cout << "mysql>";
        std::cout.flush();
    }
}

void select_test(MYSQL *mysql)
{
    std::string sql;
    sql = "select * from test";
    int ret = mysql_query(mysql, sql.c_str());
    if (ret == 0)
    {
        MYSQL_RES *res = mysql_store_result(mysql);
        if (res == nullptr)
        {
            std::cout << "mysql_store_result failed\n";
        }
        else
        {
            int row_num = mysql_num_rows(res);
            int field_num = mysql_num_fields(res);
            // 打印列名
            MYSQL_FIELD *field;
            while ((field = mysql_fetch_field(res)) != NULL)
            {
                std::cout << field->name << " ";
            }
            std::cout << std::endl;
            // 打印表数据
            for (int i = 0; i < row_num; ++i)
            {
                MYSQL_ROW row = mysql_fetch_row(res);
                for (int j = 0; j < field_num; j++)
                {
                    std::cout << row[j] << " ";
                }
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::cout << mysql_error(mysql) << std::endl;
    }
}

int main()
{
    MYSQL *mysql = mysql_init(nullptr);
    mysql = mysql_real_connect(mysql, "47.108.135.233", "mufeng", "599348181", "conn", 3306, nullptr, 0);
    if (nullptr == mysql)
    {
        std::cout << "connect failed\n";
        exit(1);
    }
    std::cout << "connect success\n";
    mysql_set_character_set(mysql, "utf8");

    client(mysql);
    // select_test(mysql);

    mysql_close(mysql);
    return 0;
}