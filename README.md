# For Linux User Space Application
- 學習用mariadb參考資料
- 型別表請參考`maria_db.png`

| 函式 / 型別                | 功能               | 常見用法                                                                      |
| ---------------------- | ---------------- | ------------------------------------------------------------------------- |
| `MYSQL *conn`          | MariaDB 連線物件     | `MYSQL *conn;`                                                            |
| `mysql_init()`         | 初始化連線物件          | `conn = mysql_init(NULL);`                                                |
| `mysql_real_connect()` | 連線到 MariaDB      | `mysql_real_connect(conn, "localhost", "user", "pwd", "db", 0, NULL, 0);` |
| `mysql_query()`        | 執行 SQL 指令        | `mysql_query(conn, sql);`                                                 |
| `mysql_error()`        | 取得錯誤訊息           | `printf("%s", mysql_error(conn));`                                        |
| `MYSQL_RES *result`    | 儲存 `SELECT` 查詢結果 | `MYSQL_RES *result;`                                                      |
| `mysql_store_result()` | 取得 `SELECT` 結果   | `result = mysql_store_result(conn);`                                      |
| `MYSQL_ROW row`        | 代表查詢結果中的一列       | `MYSQL_ROW row;`                                                          |
| `mysql_fetch_row()`    | 一列一列讀取結果         | `row = mysql_fetch_row(result);`                                          |
| `mysql_free_result()`  | 釋放查詢結果記憶體        | `mysql_free_result(result);`                                              |
| `mysql_close()`        | 關閉 MariaDB 連線    | `mysql_close(conn);`                                                      |
