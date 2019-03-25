#include <string>

class wiku_key
{
	unsigned  key[3],key0[3],adaptor,timer; //96 bits
    void iterate();
public:
    wiku_key(unsigned k0,unsigned k1, unsigned k2);
    wiku_key(unsigned k0,unsigned k1, unsigned k2, unsigned ad);
	unsigned char transfer(unsigned char chr);
	void reset(unsigned ad);
	void reset();
	int getad(){return adaptor;}
};

class crypt
{
    char type; // 0:plain 1:encrypted 2:encrypted text
    std::string path;
    wiku_key* key;
public:
    crypt(unsigned k0=0,unsigned k1=0, unsigned k2=0);
    crypt(std::string pass);
    void load(std::string file);
    void code(bool silent=0, int cond=0);
    ~crypt(){delete key;}
};
