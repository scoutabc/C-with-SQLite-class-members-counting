#include<stdio.h>
#include<wchar.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<iconv.h>
#include"create.h"
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

int main(int argc,char *argv[]) {
    sqlite3 *db;
    sqlite3_stmt* stmt;
    int rc = sqlite3_open("student.db",&db);
    if (rc != SQLITE_OK) {
        fprintf(stderr,"Cannot open this database.");
    }
    create_message(db,stmt);
    return 0;
}