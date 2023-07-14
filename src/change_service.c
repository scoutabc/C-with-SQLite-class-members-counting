#include <locale.h>
#include <stdio.h>
#include <wchar.h>
#include "../include/other.h"
#include "../include/sqlite3.h"
// 更改或添加服务信息
void change_service(sqlite3 *db, sqlite3_stmt *stmt)
{
    int saving, rc;
    wchar_t question[300];
    char query[300];
    wcscpy(question, L"目前有以下服务:");
    printf("%ls\n", question);
    sprintf(query, "SELECT * FROM Service;");
    sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    int id;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        id = sqlite3_column_int(stmt, 0);
        wchar_t *service_name = (wchar_t *)sqlite3_column_text16(stmt, 1);
        wchar_t *price = (wchar_t *)sqlite3_column_text16(stmt, 2);
        printf("%d  %ls  %ls\n", id, service_name, price);
    }

    wcscpy(question, L"您是要添加还是更改服务信息(1.添加 2.更改)");
    printf("%ls", question);
    int add_or_change;
    scanf("%d", &add_or_change);
    if (add_or_change == 1)
    {
        wchar_t addition_service[30];
        wchar_t price[30];
        do
        {
            wcscpy(question, L"请输入您要添加的服务名称:");
            printf("%ls", question);
            scanf("%ls", addition_service);
            wcscpy(question, L"请输入该服务的单价(加单位):");
            printf("%ls", question);
            scanf("%ls", price);
            swprintf(question, 300, L"您确定吗(%d  %ls  %ls,确定输入1,否则输入0)?\n", id + 1, addition_service, price);
            printf("%ls", question);
            scanf("%d", &saving);
        } while (saving != 1);
        sprintf(query, "INSERT INTO Service VALUES(%d,?,?);", id + 1);
        sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        sqlite3_bind_text16(stmt, 1, addition_service, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text16(stmt, 2, price, -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    else if (add_or_change == 2)
    {
        int service_id;
        wcscpy(question, L"您要更改哪个服务(请输入服务编号):");
        printf("%ls", question);
        scanf("%d", &service_id);
        wchar_t new_service_name[30];
        wchar_t new_price[20];
        do
        {
            wcscpy(question, L"请输入更改后的服务名称:");
            printf("%ls", question);
            scanf("%ls", new_service_name);
            wcscpy(question, L"请输入更改后的服务单价(加单位):");
            printf("%ls", question);
            scanf("%ls", new_price);
            wcscpy(question, L"您确定吗(确定输入1,否则输入0):");
            printf("%ls", question);
            scanf("%d", &saving);
        } while (saving != 1);
        sprintf(query, "UPDATE Service SET name = ?,price = ? WHERE id = %d;",service_id);
        sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
        sqlite3_bind_text16(stmt,1,new_service_name,-1,SQLITE_TRANSIENT);
        sqlite3_bind_text16(stmt,2,new_price,-1,SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    wcscpy(question,L"以下为您更改后的服务信息:");
    printf("%ls\n",question);
    sprintf(query,"SELECT * FROM Service;");
    sqlite3_prepare_v2(db,query,-1,&stmt,NULL);
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int service_id = sqlite3_column_int(stmt,0);
        wchar_t *service_name = (wchar_t *)sqlite3_column_text16(stmt,1);
        wchar_t *price = (wchar_t *)sqlite3_column_text16(stmt,2);
        printf("%d  %ls  %ls\n",service_id,service_name,price);
    }
}