#include<stdio.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include"create.h"

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
    do
    {
        wcscpy(question,L"您班级目前提供以下服务:");
        sprintf(query,"SELECT * FROM Service");
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        printf("%ls\n",question);
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        {
            int service_id = sqlite3_column_int(stmt,0);
            wchar_t *service_name = (wchar_t*)sqlite3_column_text16(stmt,1);
            double service_price = sqlite3_column_double(stmt,2);
            printf("%d  %ls  %g\n",service_id,service_name,service_price);
        }
        sqlite3_finalize(stmt);
        wcscpy(question,L"请输入您本次要统计的项目:");
        printf("%ls",question);
    } while (saving != 1);
    
}