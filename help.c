#include<stdio.h>
#include<wchar.h>
#include<locale.h>
#include"create.h"

//It can print usage
void do_help() {
    setlocale(LC_ALL,"");
    wchar_t usage[] =  L"这个命令可以进行中小学校内数据统计.\n\n"\
                       L"create [-a/--add] [-c/--change] [number-of-new-students] [-h/--help] [-l/--list] [-n/--count]\n\n"\
                       L"-a, --add              添加新的班级,以及该班级的学生\n"\
                       L"-c, --change           改变班级,学生和服务信息\n"\
                       L"-h, --help             显示该命令的使用说明\n"\
                       L"-l, --list             列出当前学生的统计数据\n"\
                       L"-n, --count            对指定班级学生进行指定服务的数据统计(默认为该选项)\n"\
                       L"以上为该命令的使用说明";
    printf("%ls\n",usage);
}