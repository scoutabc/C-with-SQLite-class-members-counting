#include<locale.h>
#include<stdio.h>
#include<wchar.h>
#include"create.h"
#include"sqlite3.h"
//更改,删除或添加学生信息
void change_student(sqlite3_stmt *stmt,sqlite3 *db) {
    int saving;
    wchar_t question[300];
    char query[300];
    int num;
    wcscpy(question,L"请输入您要更改的学生们所在的班级现在的班级号码:");
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
    wcscpy(question,L"您是否需要添加学生(只能添加到最后)");
    printf("%ls\n");
    scanf("%d",&saving);
    int count_of_students;
    sprintf(query,"SELECT COUNT(*) FROM Students WHERE class_id = %d;",num);
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    int rc = sqlite3_step(stmt);
    count_of_students = sqlite3_column_int(stmt,0);
    if (saving == 1) {
        wcscpy(question,L"您需要添加多少个学生?");
        printf("%ls",question);
        int the_count_of_addition;
        scanf("%d",&the_count_of_addition);
        for (int i = 1; i <= the_count_of_addition; i++) {
            wchar_t student_name[20];
            printf("%d:",count_of_students + i);
            scanf("%ls",student_name);
            wcscpy(question,L"您确定吗?(确定输入1,否则输入0)");
            printf("%ls",question);
            scanf("%d",&saving);
            if (saving == 1) {
                sprintf(query,"INSERT INTO Students VALUES(%d,?,%d);",count_of_students + i,num);
                sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
                sqlite3_bind_text16(stmt,1,student_name,-1,SQLITE_TRANSIENT);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }else {
                i--;
                continue;
            }
        }
    }
    wcscpy(question,L"您是否需要更改学生信息(不是删除)");
    printf("%ls",question);
    scanf("%d",&saving);
    if (saving == 1) {
        int the_count_of_change;
        wcscpy(question,L"以下为您班同学现在的名称:");
        printf("%ls",question);
        sprintf(query,"SELECT name FROM Students;");
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            wchar_t *student_name = (wchar_t*)sqlite3_column_text16(stmt,0);
            printf("%ls\n",student_name);
        }
        sqlite3_finalize(stmt);
        wcscpy(question,L"您需要更改几名学生?");
        printf("%ls",question);
        scanf("%d",&the_count_of_change);
        for (int i = 1; i < the_count_of_change;i++) {
            wchar_t new_student_name[20];
            int student_id;
            wcscpy(question,L"请输入学生编号:");
            printf("%ls",question);
            scanf("%d",&student_id);
            wcscpy(question,L"请重新输入该学生的名称:");
            printf("%ls",question);
            scanf("%ls",new_student_name);
            wcscpy(question,L"您确定吗?");
            printf("%ls",question);
            scanf("%d",saving);
            if (saving != 1) {
                
            }
        }
    }
}