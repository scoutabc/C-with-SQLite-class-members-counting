#include<stdio.h>
#include<wchar.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<iconv.h>//When we gcc -o this file,we must plus -liconv.
//This is a part of this project, it can create class and student information to tables.

//We will use this function to turn gbk string to utf8 string.
int gbk_to_utf8(char *inbuf,int inlen,char *outbuf,int outlen) {
    iconv_t cd; //That is conversion codes
    int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;
    cd = iconv_open("UTF-8","GB2312");
    if (cd == 0) {
        return -1;
    }
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen) == -1) {
        return -1;
    }
    iconv_close(cd);
    return 0;
}

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

int main(void) {
    setlocale(LC_ALL,"");
    sqlite3 *db;
    int rc = sqlite3_open("student.db",&db);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Cannot open database:%s\n",sqlite3_errmsg(db));
    }
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
            "price INTEGER NOT NULL" \
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
            "date DATE NOT NULL," \
            "duration INTEGER NOT NULL" \
            ");");
    do_query(query,db);
    //Then users can create class information
    wchar_t question[] = L"请输入班主任姓名：";
    wchar_t name[30];
    printf("%ls",question);
    scanf("%ls",name);
    int num;
    wcscpy(question,L"请输入班级号码：");
    printf("%ls",question);
    scanf("%d",&num);
    wchar_t school[30];
    wchar_t class[30];
    wcscpy(question,L"请输入学校及班级名称：");
    printf("%ls",question);
    scanf("%ls%ls",school,class);
    char *zErrmsg;
    char utf8_query[1000];
    sprintf(query,"INSERT INTO Class VALUES(%d,'%ls','%ls','%ls');",num,class,name,school);
    gbk_to_utf8(query,1000,utf8_query,1000);
    do_query(utf8_query,db);
    wcscpy(question,L"您班有几名同学？");
    printf("%ls",question);
    int number_of_people;
    wchar_t student_name[20];
    scanf("%d",number_of_people);
    for (int i = 1; i <= number_of_people; i++) {
        printf("%d:",i);
        scanf("%ls",student_name);
        sprintf(query,"INSERT INTO Students VALUES(%d,'%ls',%d);",i,student_name,num);
        gbk_to_utf8(query,1000,utf8_query,1000);
        do_query(utf8_query,db);
    }
    sqlite3_close(db);
    return 0;
}