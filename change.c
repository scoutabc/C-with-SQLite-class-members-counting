#include<locale.h>
#include<stdio.h>
#include<wchar.h>
#include"create.h"
#include"sqlite3.h"

int is_wchar_t_same(wchar_t *str1,wchar_t *str2) {
    if (wcslen(str1) != wcslen(str2)) {
        return 1;
    }
    for (int i = 0; i < wcslen(str1); i++) {
        if (str1[i] != str2[i]) {
            return 1;
        }
    }
    return 0;
}

void do_query_with_prepare(sqlite3 *db,sqlite3_stmt *stmt,char *query) {
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

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