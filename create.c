#include<stdio.h>
#include<wchar.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include"create.h"
//This is a part of this project, it can create class and student information to tables.

//We will use this function to do some sql queries
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

void create_message(sqlite3 *db,sqlite3_stmt *stmt) {
    setlocale(LC_ALL,"");
    //I create five tables in that database
    char query[1000] = "CREATE TABLE IF NOT EXISTS Students(" \
                  "id INTEGER NOT NULL," \
                  "name CHAR[20] NOT NULL," \
                  "class_id INTEGER NOT NULL" \
                  ");";
    do_query(query,db);
    strcpy(query,"CREATE TABLE IF NOT EXISTS Class(" \
            "id INTEGER NOT NULL," \
            "name CHAR[30] NOT NULL," \
            "teacher CHAR[20] NULL," \
            "school CHAR[50] NULL" \
            ");");
    do_query(query,db);
    strcpy(query,"CREATE TABLE IF NOT EXISTS Service(" \
            "id INTEGER NOT NULL," \
            "name CHAR[30] NOT NULL," \
            "price REAL NOT NULL" \
            ");");
    do_query(query,db);
    strcpy(query,"CREATE TABLE IF NOT EXISTS Class_supply_service(" \
            "class_id INTEGER NOT NULL," \
            "service_id INTEGER NOT NULL" \
            ");");
    do_query(query,db);
    strcpy(query,"CREATE TABLE IF NOT EXISTS Student_join_service(" \
            "class_id INTEGER NOT NULL," \
            "student_id INTEGER NOT NULL,"\
            "service_id INTEGER NOT NULL," \
            "date TEXT NOT NULL," \
            "duration INTEGER NOT NULL" \
            ");");
    do_query(query,db);
    //Then users can create class information
    int saving,num,rc;
    wchar_t question[300];
    wchar_t name[30];
    wchar_t school[30];
    wchar_t class[30];
    do {
        wcscpy(question,L"请输入班主任姓名:");
        printf("%ls",question);
        scanf("%ls",name);
        wcscpy(question,L"请输入班级号码:");
        printf("%ls",question);
        scanf("%d",&num);
        wcscpy(question,L"请输入学校及班级名称:");
        printf("%ls",question);
        scanf("%ls%ls",school,class);
        snwprintf(question,150,L"班级号码：%d,班级名称:%ls,班主任名称:%ls,学校名称:%ls",num,class,name,school);
        printf("%ls\n",question);
        wcscpy(question,L"您确定保存?(保存输入1,否则输入0)");
        printf("%ls",question);
        scanf("%d",&saving);
    }while(saving == 0);
    sprintf(query,"INSERT INTO Class VALUES(?,?,?,?);");
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    sqlite3_bind_int(stmt,1,num);
    sqlite3_bind_text16(stmt,2,class,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt,3,name,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text16(stmt,4,school,-1,SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sprintf(query,"SELECT * FROM Service");
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    wcscpy(question,L"目前有以下几种服务:");
    printf("%ls\n",question);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        int service_id = sqlite3_column_int(stmt,0);
        wchar_t *service_name = (wchar_t*)sqlite3_column_text16(stmt,1);
        double service_price = sqlite3_column_double(stmt,2);
        printf("%d  %ls  %g\n",service_id,service_name,service_price);
    }
    sqlite3_finalize(stmt);
    int times;
    wcscpy(question,L"您班提供几项服务?(输入服务内容时请输入编号)");
    printf("%ls",question);
    scanf("%d",&times);
    for (int i = 1; i <= times; i++) {
        int class_service_id;
        printf("%d:",i);
        scanf("%d",&class_service_id);
        wcscpy(question,L"您确定?(确定输入1,否则输入0)");
        printf("%ls",question);
        scanf("%d",&saving);
        if (saving != 1) {
            i--;
            continue;
        }
        sprintf(query,"INSERT INTO Class_supply_service VALUES(?,?);");
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_bind_int(stmt,1,num);
        sqlite3_bind_int(stmt,2,class_service_id);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    wchar_t student_name[20];
    wcscpy(question,L"您班有几名同学?");
    printf("%ls",question);
    int number_of_people;
    scanf("%d",&number_of_people);
    for (int i = 1; i <= number_of_people; i++) {
        printf("%d:",i);
        scanf("%ls",student_name);
        wcscpy(question,L"确定保存?(保存输入1,否则输入其他任意字符)");
        printf("%ls",question);
        char saving;
        while (getchar() != '\n');
        scanf("%c",&saving);
        if (saving != '1'){
            i = i - 1;
            continue;
        }
        sprintf(query,"INSERT INTO Students VALUES(?,?,?);");
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_bind_int(stmt,1,i);
        sqlite3_bind_text16(stmt,2,student_name,-1,SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt,3,num);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
}