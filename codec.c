#include "codec.h"

#include "macro.h"

static u8 table[0x100]={
#define __digit(x) [x]=x-'0'
#define __alpha(x) [x]=x-'a'+10
#define __ALPHA(x) [x]=x-'A'+10
  __digit('0'),
  __digit('1'),
  __digit('2'),
  __digit('3'),
  __digit('4'),
  __digit('5'),
  __digit('6'),
  __digit('7'),
  __digit('8'),
  __digit('9'),
  __alpha('a'),
  __alpha('b'),
  __alpha('c'),
  __alpha('d'),
  __alpha('e'),
  __alpha('f'),
  __ALPHA('A'),
  __ALPHA('B'),
  __ALPHA('C'),
  __ALPHA('D'),
  __ALPHA('E'),
  __ALPHA('F'),
#undef __ALPHA
#undef __alpha
#undef __digit
};
static u8 hex[]="0123456789abcdef";

#if 0
static __ctor void init(){
    u8 i;
    u8*p=&table[48];
    for(i=0;i<10;*p++=i++); /*0-9*/
    p=&table[64+1];
    for(i=10;i<16;*p++=i++); /*A-F*/
    p=&table[96+1];
    for(i=10;i<16;*p++=i++); /*a-f*/
}
#endif

u8*bytearray_encode(u8*out,u8*in,int len){
    u8*r,*e;
    r=out;
    for(e=in+len;in<e;in++){
        *out++=hex[*in>>4];
        *out++=hex[*in&0xf];
    }
    return r;
}

u8*bytearray_decode(u8*out,u8*in,int len){
    u8*r,*e;
    r=out;
    for(e=in+len;in<e;in+=2){
        *out++=(table[in[0]]<<4)|table[in[1]];
    }
    return r;
}

#include <openssl/des.h>

static u8*des_cbc(u8*out,u8*in,int len,const char*secret,int t){
    DES_cblock key,iv={};
    DES_key_schedule ks={};
    DES_string_to_key(secret,&key);
    if(DES_set_key_checked(&key,&ks))
        return 0;
    DES_ncbc_encrypt(in,out,len,&ks,&iv,t);
    return out;
}

u8*bytearray_encrypt(u8*out,u8*in,int len,const char*secret){
    return des_cbc(out,in,len,secret,DES_ENCRYPT);
}

u8*bytearray_decrypt(u8*out,u8*in,int len,const char*secret){
    return des_cbc(out,in,len,secret,DES_DECRYPT);
}

#if TEST_CODEC
#include <unistd.h>
#include <fcntl.h>

#include "misc.h"

MAIN_EX(argc,argv){
#define BUFLEN 512
    if(argc<4) /*(1)input-file,(2)output-file,(3)key*/
        return 1;
    int fd1 __scoped_guard(closefd)=open(argv[1],O_RDONLY);
    if(fd1==-1)
        return 2;
    char buf1[BUFLEN];
    char buf2[BUFLEN/2];
    u8*in=(u8*)&buf1[0],*out=(u8*)&buf2[0],*t;
    int nr;
    nr=(int)read(fd1,in,BUFLEN);
    int fd2 __scoped_guard(closefd)=open(argv[2],O_WRONLY|O_CREAT,0666);
    if(fd2==-1)
        return 3;
    bytearray_decode((u8*)out,(u8*)in,nr);
    SWAP(in,out,t);
    nr/=2;
    bytearray_decrypt(out,in,nr,argv[3]);
    write(fd2,out,nr);
    return 0;
}
#endif
