#include<stdio.h>
#include<wchar.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>

void do_query(char *query,sqlite3 *db) {
    char *zErrMsg;
    int rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        exit(1);
    }
}

int main(void) {
    setlocale(LC_ALL,"");
    sqlite3 *db;
    int rc = sqlite3_open("student.db",&db);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Cannot open database:%s\n",sqlite3_errmsg(db));
    }
    char *query = "CREATE TABLE IF NOT EXISTS Students(" \
                  "id INTEGER NOT NULL," \
                  "name CHAR[20] NOT NULL," \
                  "class_id INTEGER NOT NULL" \
                  ");";
    do_query(query,db);
    query = "CREATE TABLE IF NOT EXISTS Class(" \
            "id INTEGER NOT NULL," \
            "name CHAR[30] NOT NULL," \
            "teacher CHAR[20] NULL," \
            "school CHAR[50] NULL" \
            ");";
    do_query(query,db);
    query = "CREATE TABLE IF NOT EXISTS Service(" \
            "id INTEGER NOT NULL," \
            "name CHAR[30] NOT NULL," \
            "price INTEGER NOT NULL" \
            ");";
    do_query(query,db);
    query = "CREATE TABLE IF NOT EXISTS Class_supply_service(" \
            "class_id INTEGER NOT NULL," \
            "service_id INTEGER NOT NULL" \
            ");";
    do_query(query,db);
    query = "CREATE TABLE IF NOT EXISTS Student_join_service(" \
            "class_id INTEGER NOT NULL," \
            "student_id INTEGER NOT NULL,"\
            "service_id INTEGER NOT NULL," \
            "date DATE NOT NULL," \
            "duration INTEGER NOT NULL" \
            ");";
    do_query(query,db);
    wchar_t question[] = L"请输入班主任姓名：";
    wchar_t name[30];
    printf("%ls",question);
    scanf("%ls",name);
    int num;
    wcscpy(question,L"请输入班级号码：");
    printf("%ls",question);
    scanf("%d",&num);
    wchar_t school[30];
    wchar_t class[30];
    wcscpy(question,L"请输入学校及班级名称：");
    printf("%ls",question);
    scanf("%ls%ls",school,class);
    sprintf(query,"INSERT INTO Class VALUES(%d,'%ls','%ls','%ls');",&num,class,name,school);
    rc = sqlite3_exec(db,query,NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Sql error:%s\n",zErrmsg);
        sqlite3_free(zErrmsg);
        sqlite3_close(db);
        exit(1);
    }
    sqlite3_close(db);
    return 0;
}