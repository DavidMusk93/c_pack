#include <unistd.h>
#include <fcntl.h>

#include "macro.h"
#include "type.h"
#include "io.h"
#include "misc.h"
#include "log.h"
#include "codec.h"

enum{
#define __license_pkg(x,n) LICENSEPKG_##x=n
    __license_pkg(ENT,1),
    __license_pkg(ADV,2),
#undef __license_pkg
};

enum{
#define __license_type(x,n) LICENSETYPE_##x=n
    __license_type(DEMO,1),
    __license_type(POC,2),
    __license_type(PRODUCT,3),
    __license_type(INTERNAL,4),
#undef __license_type
};

enum{
#define __license_product(x,n) LICENSEPRODUCT_##x=n
    __license_product(DL,0),
    __license_product(CB,1),
#undef __license_product
};

enum{
#define __field_len(x,n) FIELDLEN_##x=n
    __field_len(VER,2),
    __field_len(NAME,10),
    __field_len(NODES,4),
    __field_len(EXPIRE,4),
    __field_len(PKG,4),
    __field_len(TYPE,4),
    __field_len(RSV1,4),
    __field_len(ID,12), /*time(8)+nonce(4)*/
    __field_len(FEATURE,2),
    __field_len(PRODUCT,4),
    __field_len(RSV2,14),
#undef __filed_len
};

enum{
#define __field_offset(x,p) FIELDOFFSET_##x=FIELDOFFSET_##p+FIELDLEN_##p
    FIELDOFFSET_VER=0,
    __field_offset(NAME,VER),
    __field_offset(NODES,NAME),
    __field_offset(EXPIRE,NODES),
    __field_offset(PKG,EXPIRE),
    __field_offset(TYPE,PKG),
    __field_offset(RSV1,TYPE),
    __field_offset(ID,RSV1),
    __field_offset(FEATURE,ID),
    __field_offset(PRODUCT,FEATURE),
    __field_offset(RSV2,PRODUCT),
    __field_offset(TOTAL,RSV2),
#undef __field_offset
};

struct __packed st_license_t{
    i16 ver;
    i8 name[FIELDLEN_NAME];
    i32 nodes;
    i32 expire;
    i32 pkg;
    i32 type;
    i8 rsv1[FIELDLEN_RSV1];
    union{
        i8 id[FIELDLEN_ID];
        struct __packed{
            i64 ts;
            i32 nonce;
        };
    };
    i16 feature;
    i32 product;
    union __packed{
        i8 rsv2[FIELDLEN_RSV2];
        u32 modules[1];
    };
};

MAIN_EX(argc,argv){
#define BUFLEN 256
#define NEVEREXPIRE 99999
    if(argc<3) /*(1)input-file,(2)key*/
        return 1;
    int fd __scoped_guard(closefd)=open(argv[1],O_RDONLY);
    if(fd==-1){
        return 2;
    }
    struct st_license_t li;
    char buf[BUFLEN];
    int nr;
    nr=(int)read(fd,buf,sizeof buf);
    struct st_unpacker_t u;
    unpacker_init(&u,buf,buf+nr);
#define UNPACK(x) u.unpack(&u,&li.x,sizeof(li.x))
    if(UNPACK(ver)
      && UNPACK(name)
      && UNPACK(nodes)
      && UNPACK(expire)
      && UNPACK(pkg)
      && UNPACK(type)
      && UNPACK(rsv1)
      && (li.ver<2
        || (UNPACK(id)
        && UNPACK(feature)
        && UNPACK(product)
        && UNPACK(rsv2)))){
        LOGINFO("(1)%#x,\n"
                "(2)%.*s,\n"
                "(3)%d,\n"
                "(4)%d,\n"
                "(5)%#x,\n"
                "(6)%d,\n"
                "(7)%.*s,\n"
                "(8)%.*s,\n"
                "(9)%#x,\n"
                "(total,%d)",
                li.ver,
                FIELDLEN_NAME,li.name,
                li.nodes,
                li.expire,
                li.pkg,
                li.type,
                FIELDLEN_RSV1,li.rsv1,
                FIELDLEN_ID,li.id,
                li.feature,
                FIELDOFFSET_TOTAL
                );
    }
    LOGINFO("sizeof(struct st_license_t)=%ld",sizeof(struct st_license_t));
    if(li.expire==NEVEREXPIRE)
        return 0;
    li.expire=NEVEREXPIRE;
    char buf2[BUFLEN];
    int n=(int)sizeof(li);
    bytearray_encrypt((u8*)buf2,(u8*)&li,n,argv[2]);
    bytearray_encode((u8*)buf,(u8*)buf2,n);
    LOGINFO("%.*s",n*2,buf);
    return 0;
}
