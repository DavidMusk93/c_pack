#include "macro.h"
#include "log.h"

struct foo{
    int x;
    char y[12];
};

struct __packed bar{
    int x;
    double y;
    union{
        char a[14];
        struct __packed{
            int b;
            long c;
        };
    };
    struct __packed{
        short d;
        int e;
    };
};

enum{
    ENUM_A=-1,
    ENUM_B,
    ENUM_C=2,
    ENUM_D,
};

MAIN(){
    struct foo o;
    LOGINFO("%ld,%ld,%ld",sizeof(o.x),sizeof(o.y)/*no decay*/,sizeof(struct bar));
    LOGINFO("%d,%d,%d,%d",ENUM_A,ENUM_B,ENUM_C,ENUM_D);
}
