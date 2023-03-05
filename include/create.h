#include"sqlite3.h"
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

#define CREATE_OK 0
#define CREATE_ERROR 1

typedef struct {
    int type;
    void* value;
} value_with_type;

int create_students(sqlite3 *db) {
    char query[300] = "CREATE TABLE IF NOT EXISTS Students(" \
                  "id INTEGER NOT NULL," \
                  "name CHAR[20] NOT NULL," \
                  "class_id INTEGER NOT NULL" \
                  ");";
    char *zErrMsg;
    int rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return CREATE_ERROR;
    }
    return CREATE_OK;
}
int create_class(sqlite3 *db) {
    char query[300] = "CREATE TABLE IF NOT EXISTS Class(" \
                      "id INTEGER NOT NULL," \
                      "name CHAR[30] NOT NULL," \
                      "teacher CHAR[20] NULL," \
                      "school CHAR[50] NULL" \
                      ");";
    char *zErrMsg;
    int rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return CREATE_ERROR;
    }
    return CREATE_OK;
}
int create_service(sqlite3* db) {
    char query[300] = "CREATE TABLE IF NOT EXISTS Service(" \
                      "id INTEGER NOT NULL," \
                      "name CHAR[30] NOT NULL," \
                      "price CHAR[10] NOT NULL" \
                      ");";
    char *zErrMsg;
    int rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return CREATE_ERROR;
    }
    return CREATE_OK;
}
int create_class_supply_service(sqlite3* db) {
    char query[300] = "CREATE TABLE IF NOT EXISTS Class_supply_service(" \
                      "class_id INTEGER NOT NULL," \
                      "service_id INTEGER NOT NULL" \
                      ");";
    char *zErrMsg;
    int rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return CREATE_ERROR;
    }
    return CREATE_OK;
}
int create_student_join_service(sqlite3 *db) {
    char query[300] = "CREATE TABLE IF NOT EXISTS Student_join_service(" \
                      "class_id INTEGER NOT NULL," \
                      "student_id INTEGER NOT NULL,"\
                      "service_id INTEGER NOT NULL," \
                      "date INTEGER NOT NULL," \
                      "duration INTEGER NOT NULL" \
                      ");";
    char *zErrMsg;
    int rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return CREATE_ERROR;
    }
    return CREATE_OK;
}
void insert_to_the_table(sqlite3 *db,sqlite3_stmt *stmt,char *table_name,int column_num,...) {
    va_list argp;
    va_start(argp,column_num);
    char query[300];
    sprintf(query,"INSERT INTO %s VALUE(",table_name);
    for (int i = 0; i < column_num; i++) {
        value_with_type the_value = va_arg(argp,value_with_type);
        int the_int_value;
        char *the_string_value;
        switch (the_value.type){
        case 1:
            the_int_value = *(int*)the_value.value;
            char the_after_query[10];
            sprintf(the_after_query,"%d",the_int_value);
            strcat(query,the_after_query);
            if (i != column_num - 1) {
                strcat(query,",");
            }
            break;
        case 2:
            the_string_value = (char*)the_value.value;
            strcat(query,the_string_value);
            if (i != column_num - 1) {
                strcat(query,",");
            }
            break;
        default:
            break;
        }
    }
    va_end(argp);
    strcat(query,");");
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}