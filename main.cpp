#include<iostream>
#include"wcode.h"
#include<string>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char** argv)
{
    if(argc==1)
        return 0;
    cout<<"Enter code:"<<endl;
    char pw[15];
    char ch;
    int i=0;
    while((ch = _getch()) != '\r') {
        if(ch == '\b' && i > 0) {
            printf("\b \b");
            --i;
        }
        else if(ch != '\b' && i < 12) {
            pw[i++] = ch;
            printf("*");
        }
    }
    crypt c(pw);
    for(int i=1; i<argc; i++)
    {
        c.load(argv[i]);
        c.code();
    }
}
