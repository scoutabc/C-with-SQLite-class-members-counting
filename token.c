#include<stdio.h>
#include"sqlite3.h"

int main() {
    sqlite3 *db;
    int rc = sqlite3_open("student.db",&db);
    char *zErrMsg;
    rc = sqlite3_exec(db,"INSERT INTO Students VALUES(12,'湡',12);",NULL,NULL,&zErrMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"SQL Error:%s",zErrMsg);
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 1;
    }
    return 0;
}