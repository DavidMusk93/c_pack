#ifndef C_PLAYGROUND_IO_H
#define C_PLAYGROUND_IO_H

#include "type.h"

#define __lc(t,c) ((t)(u8)*((c)++))
#define c2u16(c,u) (\
u=__lc(u16,c),\
u|=__lc(u16,c)<<8\
)
#define c2u32(c,u) (\
u=__lc(u32,c),\
u|=__lc(u32,c)<<8,\
u|=__lc(u32,c)<<16,\
u|=__lc(u32,c)<<24\
)
#define c2u64(c,u) (\
u=__lc(u64,c),\
u|=__lc(u64,c)<<8,\
u|=__lc(u64,c)<<16,\
u|=__lc(u64,c)<<24,\
u|=__lc(u64,c)<<32,\
u|=__lc(u64,c)<<40,\
u|=__lc(u64,c)<<48,\
u|=__lc(u64,c)<<56\
)

#define __sc(c,x) *((c)++)=(u8)((x)&0xff)
#define u16toc(u,c) (\
__sc(c,u),\
__sc(c,(u)>>8)\
)
//#define u32toc(u,c) u16toc(u,c),u>>=16,u16toc(u,c)
//#define u64toc(u,c) u32toc(u,c),u>>=32,u32toc(u,c)
#define u32toc(u,c) (\
__sc(c,u),\
__sc(c,(u)>>8),\
__sc(c,(u)>>16),\
__sc(c,(u)>>24)\
)
#define u64toc(u,c) (\
__sc(c,u),\
__sc(c,(u)>>8),\
__sc(c,(u)>>16),\
__sc(c,(u)>>24),\
__sc(c,(u)>>32),\
__sc(c,(u)>>40),\
__sc(c,(u)>>48),\
__sc(c,(u)>>56)\
)

struct st_unpacker_t{
    i8*s;
    i8*e;
    bool (*unpack)(struct st_unpacker_t*self,void*p,int len);
};

void unpacker_init(struct st_unpacker_t*self,i8*s,i8*e);

#endif //C_PLAYGROUND_IO_H
