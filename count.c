#include<stdio.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include"create.h"

void count_the_students(sqlite3 *db,sqlite3_stmt *stmt) {
    setlocale(LC_ALL,"");
    int num;
    wchar_t question[300] = L"是否需要新增服务信息?(需要输入1,不需要输入0)";
    printf("%ls",question);
    scanf("%d",&num);
    if (num == 1) {
        int service_number;
        wchar_t service_name[20];
        double price;
        char ch;
        do {
            wcscpy(question,L"请输入服务号码:");
            printf("%ls",question);
            scanf("%d",&service_number);
            wcscpy(question,L"请输入服务名称:");
            printf("%ls",question);
            scanf("%ls",service_name);
            wcscpy(question,L"请输入服务单价:");
            printf("%ls",question);
            scanf("%lf",&price);
            snwprintf(question,150,L"服务号码:%d,服务名称:%ls,服务单价:%lf",service_number,service_name,price);
            printf("%ls\n",question);
            wcscpy(question,L"您确定保存(保存输入1,否则输入0)");
            printf("%ls",question);
            while (getchar() != '\n');
            scanf("%c",&ch);
        }while (ch != '1');
        char query[300];
        sprintf(query,"INSERT INTO Service VALUES(?,?,?);");
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_bind_int(stmt,1,service_number);
        sqlite3_bind_text16(stmt,2,service_name,-1,SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt,3,price);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

}