#include<stdio.h>
#include"../include/sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include"../include/other.h"
#include <time.h>
//可以统计一天的参加服务情况.
void count_the_students(sqlite3 *db,sqlite3_stmt *stmt) {
    setlocale(LC_ALL,"");
    wchar_t question[300];
    char query[300];
    int num,saving,rc;
    do
    {
        wcscpy(question,L"请输入您班级的班级号码:");
        printf("%ls",question);
        scanf("%d",&num);
        sprintf(query,"SELECT * FROM Class WHERE id = %d;",num);
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        rc = sqlite3_step(stmt);
        wchar_t *class_name = (wchar_t*)sqlite3_column_text16(stmt,1);
        if (class_name == NULL) {
            wcscpy(question,L"错误:该班级号码不存在");
            fprintf(stderr,"%ls\n",question);
            exit(1);
        }
        wchar_t *class_teacher = (wchar_t*)sqlite3_column_text16(stmt,2);
        wchar_t *school_name = (wchar_t*)sqlite3_column_text16(stmt,3);
        snwprintf(question,300,L"班级名称:%ls,班主任名称:%ls,所在学校名称:%ls",class_name,class_teacher,school_name);
        printf("%ls\n",question);
        wcscpy(question,L"您确定吗?(确定输入1,否则输入0)");
        printf("%ls",question);
        scanf("%d",&saving);
        sqlite3_finalize(stmt);
    } while (saving != 1);
    int service_id;
    do
    {
        wcscpy(question,L"您班级目前提供以下服务:");
        sprintf(query,"SELECT id,name,price FROM Class_supply_service,Service WHERE Class_supply_service.service_id = Service.id AND Class_supply_service.class_id = %d;",num);
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        printf("%ls\n",question);
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            service_id = sqlite3_column_int(stmt,0);
            wchar_t *service_name = (wchar_t*)sqlite3_column_text16(stmt,1);
            double service_price = sqlite3_column_double(stmt,2);
            printf("%d  %ls  %g\n",service_id,service_name,service_price);
        }
        sqlite3_finalize(stmt);
        wcscpy(question,L"请输入您本次要统计的项目(输入服务编号):");
        printf("%ls",question);
        scanf("%d",&service_id);
        wcscpy(question,L"您确定吗?(确定输入1,否则输入0)");
        printf("%ls",question);
        scanf("%d",&saving);
    } while (saving != 1);
    int count_of_students;
    sprintf(query,"SELECT COUNT(*) FROM Students WHERE class_id = %d;",num);
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    rc = sqlite3_step(stmt);
    count_of_students = sqlite3_column_int(stmt,0);
    for (int i = 1; i <= count_of_students; i++) {
        sprintf(query,"SELECT name FROM Students WHERE id = %d AND class_id = %d;",i,num);
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_step(stmt);
        wchar_t *student_name = (wchar_t*)sqlite3_column_text16(stmt,0);
        snwprintf(question,300,L"%d:%ls(1. 4:30~5:30, 2. 4:30~6:30, 0. 没有参加, 其余数字自动按0处理)",i,student_name);
        sqlite3_finalize(stmt);
        printf("%ls",question);
        int duration;
        scanf("%d",&duration);
        wcscpy(question,L"您确定吗?(确定输入1,否则输入0)");
        printf("%ls",question);
        scanf("%d",&saving);
        if (saving != 1) {
            i--;
            continue;
        }
        if (duration == 1 || duration == 2) {
            time_t timenow;
            time(&timenow);
            sprintf(query,"INSERT INTO Student_join_service VALUES(?,?,?,?,?);");
            sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
            sqlite3_bind_int(stmt,1,num);
            sqlite3_bind_int(stmt,2,i);
            sqlite3_bind_int(stmt,3,service_id);
            sqlite3_bind_int(stmt,4,timenow);
            sqlite3_bind_int(stmt,5,duration);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
}