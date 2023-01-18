#include<locale.h>
#include<stdio.h>
#include<wchar.h>
#include"create.h"
#include"sqlite3.h"

void change_information(sqlite3 *db,sqlite3_stmt *stmt) {
    wchar_t question[300];
    char query[300];
    wcscpy(question,L"请选择您要更改的信息内容:1.班级信息    2.学生信息    3.服务信息");
    printf("%ls\n",question);
    int information;
    scanf("%d",&information);
    if (information == 1) {
        int num;
        wcscpy(question,L"请输入您要更改的班级现在的班级号码:");
        printf("%ls",question);
        scanf("%d",&num);
        sprintf(query,"SELECT teacher FROM Class WHERE id = %d;",num);
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_step(stmt);
        wchar_t *teacher_name = (wchar_t*)sqlite3_column_text16(stmt,0);
        wcscpy(question,L"请输入该班级的班主任名称:");
        printf("%ls",question);
        wchar_t *input_teacher_name;
        scanf("%ls",input_teacher_name);
    }
}