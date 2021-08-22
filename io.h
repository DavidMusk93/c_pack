#ifndef C_PLAYGROUND_IO_H
#define C_PLAYGROUND_IO_H

#include "type.h"

struct st_unpacker_t{
    i8*s;
    i8*e;
    bool (*unpack)(struct st_unpacker_t*self,void*p,int len);
};

void unpacker_init(struct st_unpacker_t*self,i8*s,i8*e);

#endif //C_PLAYGROUND_IO_H
