#include<stdio.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include"create.h"

void count_the_students(sqlite3 *db,sqlite3_stmt *stmt) {
    setlocale(LC_ALL,"");
}