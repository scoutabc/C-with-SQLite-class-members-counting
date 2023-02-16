#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sqlite3.h"
#include "create.h"
// 将统计结果打印到输出流或者打印到csv表里

static void calculate_the_time(sqlite3_context *context, int argc, sqlite3_value **argv) {
    struct tm *info;
    time_t the_time = (time_t)sqlite3_value_int(argv[0]);
    info = localtime(&the_time);
    char *the_date;
    sprintf(the_date,"%d-%d",info -> tm_year + 1900,info -> tm_mon + 1);
    sqlite3_result_text(context,the_date,strlen(the_date),SQLITE_TRANSIENT);
}

int last_day_of_month(char *month){
    char ch_year[5];
    for (int i = 0; i < 4; i++){
        ch_year[i] = month[i];
    }
    char month_num1[2],month_num2[2];
    month_num1[0] = month[5];
    month_num1[1] = '\0';
    month_num2[0] = month[6];
    month_num2[1] = '\0';
    ch_year[4] = '\0';
    int year = atoi(ch_year);
    if (month[5] == '2' && year % 4 == 0){
        return 29;
    }else if (month[5] == '2' && year % 4 != 0){
        return 28;
    }else if ((atoi(month_num1) % 2 == 1 && month[6] == '\0' && atoi(month_num1) < 9) || month[5] == '8'){
        return 31;
    }else if (month[5] == '1' && atoi(month_num2) % 2 == 0){
        return 31;
    }else{
        return 30;
    }
}

int register_udf(sqlite3 *db) {
    return sqlite3_create_function(db,"calculate_the_time",1,SQLITE_UTF8,NULL,&calculate_the_time,NULL,NULL);
}

void list(sqlite3 *db, sqlite3_stmt *stmt){
    setlocale(LC_ALL, "");
    wchar_t question[300];
    int num, saving;
    char query[300];
    int rc;
    wcscpy(question, L"请输入您的班级号码:");
    printf("%ls", question);
    scanf("%d", &num);
    sprintf(query, "SELECT teacher FROM Class WHERE id = %d;", num);
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    sqlite3_step(stmt);
    wchar_t *teacher_name = (wchar_t *)sqlite3_column_text16(stmt, 0);
    if (teacher_name == NULL){
        wcscpy(question, L"错误:该班级不存在");
        printf("%ls", question);
        exit(1);
    }
    wcscpy(question, L"请输入该班级的班主任名称:");
    printf("%ls", question);
    wchar_t input_teacher_name[30];
    scanf("%ls", input_teacher_name);
    int result = wcscmp(teacher_name, input_teacher_name);
    if (result != 0){
        wcscpy(question, L"错误:班主任名称错误");
        printf("%ls", question);
        exit(1);
    }
    char start_date[12];
    char end_date[12];
    char month[10];
    do {
        wcscpy(question, L"您需要哪年哪一月的统计信息(请以YYYY-M/YYYY-MM为格式,否则就会出错):");
        printf("%ls", question);
        scanf("%s", month);
        strcpy(start_date,month);
        strcpy(end_date, start_date);
        strcat(start_date, "-1");
        char end_date_add[5];
        sprintf(end_date_add, "-%d", last_day_of_month(end_date));
        strcat(end_date, end_date_add);
        printf("%s~%s\n", start_date, end_date);
        wcscpy(question, L"您确定吗?");
        printf("%ls");
        scanf("%d",&saving);
    }while(saving == 0);
    int chance;
    wcscpy(question, L"请选择打印方式(1.打印到csv格式的Excel表中 2.打印到程序界面):");
    printf("%ls",question);
    scanf("%d",&chance);
    if (chance == 1) {
        FILE *fp;
        wchar_t *wmonth = (wchar_t*) month;
        char filename[20];
        sprintf(filename,"data%s.csv",month);
        printf("%s",month);
        fp = fopen(filename,"w");
        if (fp != NULL) {
            sprintf(query,"SELECT name FROM Class WHERE id = %d;",num);
            sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
            sqlite3_step(stmt);
            wchar_t *class_name = (wchar_t*)sqlite3_column_text16(stmt,0);
            snwprintf(question,300,L"月课后服务收费明细  班级(  %ls  ),,,,,,,,",class_name);
            fprintf(fp,"%s%ls\n",month,question);
            wcscpy(question,L"序号,姓名,05:30,,,06:30,,,应收合计");
            fprintf(fp,"%ls\n",question);
            wcscpy(question,L",,单价,次数,金额,单价,次数,金额,");
            fprintf(fp,"%ls\n",question);
            sprintf(query,"SELECT COUNT(*) FROM Students WHERE class_id = %d;",num);
            sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
            sqlite3_step(stmt);
            int number_of_students = sqlite3_column_int(stmt,0);
            for (int i = 1; i <= number_of_students; i++) {
                sprintf(query,"SELECT name FROM Students WHERE id = %d;",i);
                sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
                sqlite3_step(stmt);
                wchar_t *student_name = (wchar_t*)sqlite3_column_text16(stmt,0);
                fprintf(fp,"%d,%ls,4.5,",i,student_name);
                rc = register_udf(db);
                int the_times_of_530; 
                sprintf(query,"SELECT NUM(*) FROM Student_join_service WHERE calculate_the_time(date) = %s AND class_id = %d AND student_id = %d",month,num,i);
                fprintf(fp,"\n");
            }
        }
        fclose(fp);
    }
}