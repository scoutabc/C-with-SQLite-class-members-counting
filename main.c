#include<stdio.h>
#include<wchar.h>
#include"sqlite3.h"
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<iconv.h>
#include"create.h"
#include<getopt.h>

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
    static struct option options[] = {
        {"add",no_argument,NULL,'a'},
        {"change",required_argument,NULL,'c'},
        {"help",no_argument,NULL,'h'},
        {"list",no_argument,NULL,'l'},
        {NULL,0,NULL,0}
    };
    int opt_index;
    char c = getopt_long(argc,argv,"ac:hl",options,&opt_index);
    switch (c) {
        case 0:
            printf("yahu\n");
            break;
        case 'a':
            create_message(db,stmt);
            break;
        case 'c':
            break;
        case 'h':
            do_help();
            break;
        case 'l':
            break;
        default:
            break;
    }
    return 0;
}