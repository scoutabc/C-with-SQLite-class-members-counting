#include<locale.h>
#include<stdio.h>
#include<wchar.h>
#include"create.h"
#include"sqlite3.h"
//可以用于修改学生,班级或服务信息

//这个函数可以用sqlite3_prepare_v2函数的方式来执行sql命令
void do_query_with_prepare(sqlite3 *db,sqlite3_stmt *stmt,char *query) {
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

//这个函数可以用于修改信息
void change_information(sqlite3 *db, sqlite3_stmt *stmt) {
    setlocale(LC_ALL,"");
    wchar_t question[300];
    wcscpy(question,L"请选择您要更改的信息内容:1.班级信息    2.学生信息    3.服务信息");
    printf("%ls\n",question);
    int information;
    scanf("%d",&information);
    if (information == 1) {
        change_class(stmt,db);
    }else if (information == 2) {
        change_student(stmt,db);
    }
}