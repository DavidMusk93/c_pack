#include "macro.h"
#include "misc.h"
#include "log.h"

MAIN_EX(argc,argv){
    if(argc<2)
        return 1;
    FILE*fp __scoped_guard(closefp)=fopen(argv[1],"r");
    if(!fp)
        return 2;
    int nr;
#if 0
    char buf[32];
    nr=(int)fread(buf,sizeof buf,1,fp); /*count eof, return 0*/
    LOGINFO("%d,%ld",nr,*(long*)buf);
#else
    long sec;
    nr=(int)fread(&sec,1,sizeof(sec),fp);
    LOGINFO("%d,%ld",nr,sec);
#endif
    return 0;
}