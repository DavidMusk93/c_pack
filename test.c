#include "macro.h"
#include "log.h"

struct st_foo_t{
    int x;
    char y[12];
};

struct __packed st_bar_t{
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
    struct st_foo_t o;
    LOGINFO("%ld,%ld,%ld",sizeof(o.x),sizeof(o.y)/*no decay*/,sizeof(struct st_bar_t));
}