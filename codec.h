#ifndef C_PLAYGROUND_CODEC_H
#define C_PLAYGROUND_CODEC_H

#include "type.h"

u8*bytearray_encode(u8*out,u8*in,int len);
u8*bytearray_decode(u8*out,u8*in,int len);
u8*bytearray_encrypt(u8*out,u8*in,int len,const char*secret);
u8*bytearray_decrypt(u8*out,u8*in,int len,const char*secret);

#endif //C_PLAYGROUND_CODEC_H
