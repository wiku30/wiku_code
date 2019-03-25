#include "wcode.h"
#include <ctime>

static int counter;

wiku_key::wiku_key(unsigned k0,unsigned k1, unsigned k2, unsigned ad)
{
    key[0]=k0;
    key[1]=k1;
    key[2]=k2;
    key0[0]=k0;
    key0[1]=k1;
    key0[2]=k2;
    timer=0;
    counter=0;
    adaptor=ad;
}

wiku_key::wiku_key(unsigned k0,unsigned k1, unsigned k2)
{
    key[0]=k0;
    key[1]=k1;
    key[2]=k2;
    key0[0]=k0;
    key0[1]=k1;
    key0[2]=k2;
    timer=0;
    counter=0;
    adaptor = time(0) * 2333333 + counter * 23333333 + clock() * 233333333 ;
    counter++;
}

void wiku_key::reset(unsigned ad)
{
    adaptor = ad;
    for(int i=0; i<3; i++)
        key[i]=key0[i];
    timer=0;
}

void wiku_key::reset()
{
    for(int i=0; i<3; i++)
        key[i]=key0[i];
    timer=0;
}

void wiku_key::iterate()
{
    int a0,a1,a2,a3,a4,a5;
    timer++;

    for(int i=0; i<3; i++)
    {
        //modifying
        key[2]+=19971019;
        key[0]^=key[1]&19971019;
        key[1]+=key[2]*19971019;

        //shattering bijection
        a1=key[0]/65536;
        a4=key[0]%65536;
        a5=key[1]/65536;
        a2=key[1]%65536;
        a0=key[2]/65536;
        a3=key[2]%65536;
        a1^=a4;
        a5^=a0;
        a2^=a3;
        key[0]=a0*65536+a1;
        key[1]=a2*65536+a3;
        key[2]=a4*65536+a5;

        //adaptor
        adaptor += key[2];
        key[0] += adaptor ^ key[1];

        //hashing
        key[0]+= (key[1]%19971019) ^ (key[2]*19971019);
        key[1]^= (key[2]%19971019) + (key[0]*19971019);
        key[2]+= (key[0]*19971019) - (key[1]^19971019);
    }

    //introducing timer
    key[2]^= timer*19971019;
}

unsigned char wiku_key::transfer(unsigned char chr)
{
    iterate();
    unsigned char res = chr ^ ((key[0]/16384)%256);
    adaptor += (res+chr) * 19971019;
    return res;
}
