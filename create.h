#include"sqlite3.h"

int gbk_to_utf8(char *inbuf,int inlen,char *outbuf,int outlen);
void do_query(char *query,sqlite3 *db);
void create_message(sqlite3 *db);