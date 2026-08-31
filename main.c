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

# 查套件是否已安裝 ii代表已安裝
dpkg -l | grep mariadb-server
dpkg -l | grep libmariadb-dev
# 要看套件到底安裝了哪些檔案
dpkg -L mariadb-server
dpkg -L libmariadb-dev

/usr/include 底下的檔案路徑，
通常可以直接對應到 #include <...> 裡面的名稱。

*/
/*
systemctl status mariadb
systemctl start mariadb

>mariadb
ERROR 1698 (28000): Access denied for user 'pi'@'localhost'
>sudo mariadb
SELECT User, Host FROM mysql.user;

CREATE DATABASE MEME53_05;
SHOW DATABASES;
USE MEME53_05;
CREATE TABLE dht11_data (
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    temperature DECIMAL(5,2)  NULL,
    humidity DECIMAL(5,2)  NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
SHOW TABLES;
DESC dht11_data;

# INSERT INTO 表名 (欄位1, 欄位2)
# VALUES (值1, 值2);

INSERT INTO dht11_data
(temperature, humidity)
VALUES
(25.6, 68.3);
SELECT * FROM dht11_data;

//##############################################
//這裡很重要SELECT User, Host FROM mysql.user;
//'meme5305'@'localhost' 所以這個帳號只能從 MariaDB 所在的 Linux 本機登入。
//MariaDB 不是讓你手動選，而是它會根據「你從哪裡連進來」自動匹配對應的 user@host。
//##############################################

//不建議讓 C 用 root
CREATE USER 'meme5305'@'localhost'
IDENTIFIED BY '123456';
//意思是建立使用者 meme5305，而且只允許從 localhost（資料庫主機本機）登入；
//  IDENTIFIED BY 後面則是設定登入密碼。
//      只有其他電腦透過網路直接連 Raspberry Pi 的 MariaDB 時，才不能只用 localhost。
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
SELECT * FROM dht11_data;//測試查詢
*/

//完整、也更推薦的編譯方式。
//mariadb_config --cflags --libs #自動產生編譯 MariaDB C 程式需要的參數
//gcc test.c -o main $(mariadb_config --cflags --libs)

/*
MYSQL *conn;
conn = mysql_init(NULL);
mysql_real_connect(...);
mysql_query(...);
mysql_close(conn);
*/
//##################################################
//mysql_query() 是執行 SQL 指令的通用函式，不只 INSERT / UPDATE / DELETE，SELECT 也可以。
//  如果是 SELECT，還要另外把查詢結果取出來。
//##################################################
int main(void)
{
    MYSQL *conn;
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init failed\n");
        return 1;
    }
    if (mysql_real_connect(
            conn        ,"localhost",
            "meme5305"  ,   "123456",
            "MEME53_05" ,          0,
            NULL,0
        ) == NULL) {

        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // const char *sql =
    // "INSERT INTO dht11_data "
    // "(temperature, humidity) "
    // "VALUES (25.6, 68.3)";

    // if (mysql_query(conn, sql) != 0) {
    //     fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
    //     mysql_close(conn);
    //     return 1;
    // }

    const char *select_sql = "SELECT * FROM dht11_data WHERE id = 1";
    if (mysql_query(conn, select_sql) != 0) {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }
    //SELECT 查回來的值，會先放進 MYSQL_RES * 查詢結果物件，接著你再用 MYSQL_ROW 一列一列取出來。
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row; //很可能會查到很多列 沒有資料了 → 回傳 NULL
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("id = %s\n", row[0]);
        printf("temperature = %s\n", row[1]);
        printf("humidity = %s\n", row[2]);
    }
    mysql_free_result(result); //釋放 SELECT 查詢結果佔用的記憶體。

    printf("MariaDB connected!\n");

    mysql_close(conn);

    return 0;
}
