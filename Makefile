CC = gcc
TARGET = main
SRC = main.c

# 一般 user space 的 main.c
#include <mariadb/mysql.h> → 告訴編譯器函式長什麼樣
#-lmariadb → 告訴 linker 去哪裡找這些函式的實作

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) -lmariadb

clean:
	rm -f $(TARGET)
