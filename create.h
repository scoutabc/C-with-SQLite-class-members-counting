#include"sqlite3.h"
#include<iconv.h>
#include<stdlib.h>
#include<string.h>

void do_query(char *query,sqlite3 *db);
void create_message(sqlite3 *db,sqlite3_stmt* stmt);
void do_help();
void count_the_students(sqlite3 *db,sqlite3_stmt *stmt);
void change_information(sqlite3 *db,sqlite3_stmt *stmt);
void do_query_with_prepare(sqlite3 *db,sqlite3_stmt *stmt,char *query);
void change_class(sqlite3_stmt *stmt,sqlite3 *db);
void change_student(sqlite3_stmt *stmt,sqlite3 *db);