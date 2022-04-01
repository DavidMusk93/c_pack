#include "io.h"

#include <string.h>

#define UNPACKER struct unpacker

static bool unpack(UNPACKER*self,void*p,int len){
    if(self->s+len<=self->e){
        memcpy(p,self->s,len);
        self->s+=len;
        return true;
    }
    return false;
}

void unpacker_init(UNPACKER*self,i8*s,i8*e){
    self->s=s,self->e=e;
    self->unpack=&unpack;
}

#if TEST_CONVERT
#include "macro.h"
#include "log.h"

typedef char a8_t[8];

MAIN(){
    u64 x=0x1122334455667788,y;
    a8_t a;
    char*p=&a[0];
    u64toc(x,p);
    p=&a[0];
    c2u64(p,y);
    LOGINFO("%lx",y);
}
#endif
