#include "io.h"

#include <string.h>

#define UNPACKER struct st_unpacker_t

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