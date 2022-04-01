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

enum{
    MODULE_MIN=-1,
    MODULE_LOCK=0,
    MODULE_DDL, /*online*/
    MODULE_TRIGGER=2,
    MODULE_SPJ,
    MODULE_CONNECT=4,
    MODULE_AUDIT, /*security*/
    MODULE_MAC=6,
    MODULE_AUTH, /*local*/
    MODULE_CACHE=8, /*little table*/
    MODULE_HIVE,
    MODULE_TENANT=10,
    MODULE_RESTORE,
    MODULE_BINLOG=12,
    MODULE_LOB,
    MODULE_MAX=14,
};

enum{
    kSUCCESS,
    kFAILURE,
};
enum{
    MODULEOP_TEST,
    MODULEOP_ENABLE,
    MODULEOP_DISABLE,
};

struct __packed license{
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
        u8 rsv2[FIELDLEN_RSV2];
        u32 modules[1];
    };
};

int license_module_op(struct license*o,int op,int module){
    if(module<=MODULE_MIN||module>=MODULE_MAX)
        return kFAILURE;
    u8*p=&o->rsv2[module/8];
    int x=1<<(module%8);
    if(op==MODULEOP_TEST)
        return *p&x?kSUCCESS:kFAILURE;
    else if(op==MODULEOP_ENABLE)
        *p|=x;
    else if(op==MODULEOP_DISABLE)
        *p&=~x;
    else
        return kFAILURE;
    return kSUCCESS;
}

MAIN_EX(argc,argv){
#define BUFLEN 256
#define NEVEREXPIRE 99999
    if(argc<3) /*(1)input-file,(2)key*/
        return 1;
    int fd __scoped_guard(closefd)=open(argv[1],O_RDONLY);
    if(fd==-1){
        return 2;
    }
    struct license li;
    char buf[BUFLEN];
    int nr;
    nr=(int)read(fd,buf,sizeof buf);
    struct unpacker u;
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
    LOGINFO("sizeof(struct license)=%ld",sizeof(struct license));
#if 0 /*never expire is a risk*/
    if(li.expire==NEVEREXPIRE)
        return 0;
    li.expire=NEVEREXPIRE;
#endif
#ifdef DISABLE_HIVE
    license_module_op(&li,MODULEOP_DISABLE,MODULE_HIVE);
#endif
    char buf2[BUFLEN];
    int n=(int)sizeof(li);
    bytearray_encrypt((u8*)buf2,(u8*)&li,n,argv[2]);
    bytearray_encode((u8*)buf,(u8*)buf2,n);
    LOGINFO("%.*s",n*2,buf);
    return 0;
}
