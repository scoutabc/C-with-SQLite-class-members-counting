#include<stdio.h>
#include<wchar.h>
#include<stdio.h>
#include<locale.h>
#include<stdlib.h>

int main() {
    setlocale(LC_ALL,"");
    wchar_t name[10];
    wchar_t question[] = L"请输入班主任姓名：";
    printf("printf content:%S\n",question);
    //wscanf(L"%ls ",name);
    int num;
    wcscpy(question,L"请输入班级号码：");
    printf("hello:%S\n",question);
    system("pause");
    return 0;
    //wscanf(L"%d ",&num);
    //printf("%ls %d",name,num);
}