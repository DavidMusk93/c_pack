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

MAIN(){
    struct foo o;
    LOGINFO("%ld,%ld,%ld",sizeof(o.x),sizeof(o.y)/*no decay*/,sizeof(struct bar));
}
