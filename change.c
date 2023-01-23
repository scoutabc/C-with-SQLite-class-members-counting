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
    char  query[300];
    int saving;
    int num;
    wcscpy(question,L"请选择您要更改的信息内容:1.班级信息    2.学生信息    3.服务信息");
    printf("%ls\n",question);
    int information;
    scanf("%d",&information);
    if (information == 1) {
        wcscpy(question,L"请输入您要更改的班级现在的班级号码:");
        printf("%ls",question);
        scanf("%d",&num);
        sprintf(query,"SELECT teacher FROM Class WHERE id = %d;",num);
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_step(stmt);
        wchar_t *teacher_name =  (wchar_t*)sqlite3_column_text16(stmt,0);
        if (teacher_name == NULL) {
            wcscpy(question,L"错误:该班级不存在");
            printf("%ls",question);
            exit(1);
        }
        wcscpy(question,L"请输入该班级的班主任名称:");
        printf("%ls",question);
        wchar_t input_teacher_name[30];
        scanf("%ls", input_teacher_name);
        int result = wcscmp(teacher_name,input_teacher_name);
        if (result != 0) {
            wcscpy(question,L"错误:班主任名称错误");
            printf("%ls",question);
            exit(1);
        }
        wcscpy(question,L"你们班是否已经毕业?(请谨慎填写,如果您输入1,将会删除与您班级有关的所有信息)");
        printf("%ls",question);
        int is_graduate;
        scanf("%d",&is_graduate);
        if (is_graduate == 1) {
            wcscpy(question,L"请您再次确认(确定输入1,否则输入0)");
            printf("%ls",question);
            scanf("%d",&saving);
            if (saving == 1) {
                sprintf(query,"DELETE FROM Class WHERE id = %d;",num);
                do_query_with_prepare(db,stmt,query);
                sprintf(query,"DELETE FROM Students WHERE class_id = %d;",num);;
                do_query_with_prepare(db,stmt,query);
                sprintf(query,"DELETE FROM Class_supply_service WHERE class_id = %d;",num);
                do_query_with_prepare(db,stmt,query);
                sprintf(query,"DELETE FROM Student_join_service WHERE class_id = %d;",num);
                do_query_with_prepare(db,stmt,query);
                exit(0);
            }
        }
    }
}