#include<locale.h>
#include<stdio.h>
#include<wchar.h>
#include"create.h"
#include"sqlite3.h"

void change_class(sqlite3_stmt *stmt,sqlite3 *db) {
    wchar_t question[300];
    char  query[300];
    int saving;
    int num;
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
    wcscpy(question,L"您班级是否已经升年级(如果您输入1,将会更改您的班级号码,但不会更改班级名称)");
    printf("%ls",question);
    int is_up_grade;
    scanf("%d",&is_up_grade);
    if (is_up_grade == 1) {
        sprintf(query,"UPDATE Class SET id = id + 100 WHERE id = %d",num);
        do_query_with_prepare(db,stmt,query);
        sprintf(query,"UPDATE Students SET class_id = class_id + 100 WHERE class_id = %d",num);
        do_query_with_prepare(db,stmt,query);
        sprintf(query,"UPDATE Class_supply_service SET class_id = class_id + 100 WHERE class_id = %d",num);
        do_query_with_prepare(db,stmt,query);
        sprintf(query,"UPDATE Student_join_service SET class_id = class_id + 100 WHERE class_id = %d",num);
        do_query_with_prepare(db,stmt,query);
        num += 100;
    }
    wchar_t changed_teacher[20];
    wchar_t changed_name[20];
    wchar_t changed_school[30];
    wcscpy(question,L"请输入您班级的信息(更改后)");
    printf("%ls\n",question);
    wcscpy(question,L"班主任名称:");
    printf("%ls",question);
    scanf("%ls",changed_teacher);
    wcscpy(question,L"班级名称:");
    printf("%ls",question);
    scanf("%ls",changed_name);
    wcscpy(question,L"学校名称:");
    printf("%ls",question);
    scanf("%ls",changed_school);
    sprintf(query,"UPDATE Class SET school = ?,name = ?,teacher = ? WHERE id = %d",num);
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    sqlite3_bind_text16(stmt,1,changed_school,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt,2,changed_name,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt,3,changed_teacher,-1,SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}