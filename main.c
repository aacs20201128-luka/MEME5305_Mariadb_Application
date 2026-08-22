#include <stdio.h>
#include <mariadb/mysql.h>

/*
sudo apt update
sudo apt install \
    gcc \
    make \
    mariadb-server \
    libmariadb-dev
*/
/*
dpkg
-L  = List files in package
-l  = list packages
usr/include下的路徑通常就直接對應include名稱
*/
/*
systemctl status mariadb
systemctl start mariadb
>mariadb
CREATE DATABASE MEME53_05;
SHOW DATABASES;
USE MEME53_05;
CREATE TABLE TEST_TB (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    temperature DECIMAL(5,2)  NULL,
    humidity DECIMAL(5,2)  NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
SHOW TABLES;
DESC TEST_TB;
INSERT INTO TEST_TB
(temperature, humidity)
VALUES
(25.6, 68.3);
SELECT * FROM TEST_TB;

//##############################################
//這裡很重要SELECT User, Host FROM mysql.user 
//'meme5305'@'localhost' 所以這個帳號只能從 MariaDB 所在的 Linux 本機登入。
//MariaDB 不是讓你手動選，而是它會根據「你從哪裡連進來」自動匹配對應的 user@host。
//##############################################

//不建議讓 C 用 root
CREATE USER 'meme5305'@'localhost'
IDENTIFIED BY '123456';
//意思是建立使用者 meme5305，而且只允許從 localhost（資料庫主機本機）登入；IDENTIFIED BY 後面則是設定登入密碼。
GRANT SELECT, INSERT, UPDATE, DELETE
ON MEME53_05.*
TO 'meme5305'@'localhost';
FLUSH PRIVILEGES;

//##################################################
GRANT ALL PRIVILEGES ON MEME53_05.*
TO 'meme5305'@'192.168.39.101'; 特權
//##################################################


SHOW GRANTS FOR 'meme5305'@'localhost';//檢查權限
EXIT;//退出
mariadb -u meme5305 -p MEME53_05//登入測試
SELECT * FROM TEST_TB;//測試查詢
*/
/*
MYSQL *conn;
conn = mysql_init(NULL);
mysql_real_connect(...);
mysql_query(...);
mysql_close(conn);
*/

void main(void){

    MYSQL *conn;
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init failed\n");
        return 1;
    }
    if (mysql_real_connect(
            conn,
            "localhost",
            "meme5305",
            "123456",
            "MEME53_05",
            0,
            NULL,
            0
        ) == NULL) {

        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // const char *sql =
    // "INSERT INTO TEST_TB "
    // "(temperature, humidity) "
    // "VALUES (25.6, 68.3)";

    // if (mysql_query(conn, sql) != 0) {
    //     fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
    //     mysql_close(conn);
    //     return 1;
    // }
    //不是，mysql_query() 是 「執行 SQL 指令」的通用函式，不只新增、修改、刪除

    const char *select_sql = "SELECT * FROM TEST_TB WHERE id = 1";
    if (mysql_query(conn, select_sql) != 0) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }
    //SELECT 查回來的值，會先放進 MYSQL_RES * 查詢結果物件，接著你再用 MYSQL_ROW 一列一列取出來。
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row;
    //很可能會查到很多列 沒有資料了 → 回傳 NULL
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("id = %s\n", row[0]);
        printf("temperature = %s\n", row[1]);
        printf("humidity = %s\n", row[2]);
    }
    mysql_free_result(result);

    printf("MariaDB connected!\n");

    mysql_close(conn);

    //mariadb_config --cflags --libs
    //gcc test.c -o main $(mariadb_config --cflags --libs)
    return 0;
}
