#include<stdio.h>
#include<wchar.h>
#include"create.h"

void do_help() {
    wchar_t *usage =  "这个命令可以进行中小学校内数据统计.\n\n"\
                      "create [-a/--add] [-c/--change] [number-of-new-students] [-h/--help] [-l/--list] [-n/--count]\n\n"\
                      "-a, --add              添加新的班级,以及该班级的学生\n"\
                      "-c, --change           改变班级的学生名单(添加或删除学生)\n"\
                      "-h, --help             显示该命令的使用说明\n"\
                      "-l, --list             列出当前学生的统计数据\n"\
                      "-n, --count            对指定班级学生进行制定服务的数据统计(默认)\n"\
                      "以上为该命令的使用说明"
    printf("%ls",usage);
}