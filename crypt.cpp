#include "wcode.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

crypt::crypt(unsigned k0,unsigned k1, unsigned k2)
{
    key=new wiku_key(k0,k1,k2);
}

crypt::crypt(string pass)
{
    pass.resize(12);
    int k0,k1,k2;
    k0=unsigned(pass[0])*16777216 + unsigned(pass[1])*65536 + unsigned(pass[2])*256 + unsigned(pass[3]) ;
    k1=unsigned(pass[4])*16777216 + unsigned(pass[5])*65536 + unsigned(pass[6])*256 + unsigned(pass[7]) ;
    k2=unsigned(pass[8])*16777216 + unsigned(pass[9])*65536 + unsigned(pass[10])*256 + unsigned(pass[11]) ;
    key=new wiku_key(k0,k1,k2);
}

void crypt::load(std::string file)
{
    path=file;
    if(path.length()>6 && path.substr(path.length()-6,6)==".wtext")
        type=2;
    else if(path.length()>5 && path.substr(path.length()-5,5)==".wiku")
        type=1;
    else
        type=0;
}

void crypt::code(bool silent, int cond) //1: only encrypt 2:only decrypt
{
    FILE *fp = fopen(path.c_str(), "rb");
    FILE *tp;
    string tgtpath;
    bool flag=0;
    if(!fp)
    {
        if(!silent)
            cout<<"Failure in opening: "<<path<<endl;
        return;
    }
    if(type == 2)
    {
        unsigned ad=0;
        for(int i=0; i<4; i++)
        {
            ad*=256;
            ad+=fgetc(fp);
        }
        key->reset(ad);
        cout<<endl<<endl;
        while(1)
        {
            unsigned char ch=fgetc(fp);
            if(feof(fp))
                break;
            cout.put(key->transfer(ch));    
        }
        cout<<endl<<endl;
        system("pause");
        return;
    }
    else if(type && (cond!=1)) //encrypted
    {
        tgtpath = path.substr(0, path.length()-5);
        if(fopen(tgtpath.c_str(),"rb"))
        {
            if(!silent)
                cout<<tgtpath<<" already exists!"<<endl;
            return;
        }
        flag=1;
        unsigned ad=0;
        for(int i=0; i<4; i++)
        {
            ad*=256;
            ad+=fgetc(fp);
        }
        key->reset(ad);
        tp = fopen(tgtpath.c_str(),"wb");
    }
    else if(cond!=2)  //plain
    {
        flag=1;
        tgtpath = path + ".wiku";
        tp = fopen(tgtpath.c_str(),"wb");
        unsigned tmp=key->getad();
        for(int i=0; i<4; i++)
        {
            fputc(tmp>>(24-8*i)&255, tp);
        }
        key->reset();
    }
    while(flag && type!=2)
    {
        unsigned char ch=fgetc(fp);
        if(feof(fp))
            break;
        fputc(key->transfer(ch),tp);
    }
    fclose(fp);
    if(flag)
        fclose(tp);
}
