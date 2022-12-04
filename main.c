#include<stdio.h>
#include<wchar.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<iconv.h>
#include"create.h"

int main(int argc,char *argv[]) {
    sqlite3 *db;
    int rc = sqlite3_open("student.db",&db);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Cannot open this database.");
    }
    create_message(db);
    return 0;
}