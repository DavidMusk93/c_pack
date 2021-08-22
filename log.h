#ifndef C_PLAYGROUND_LOG_H
#define C_PLAYGROUND_LOG_H

#include <stdio.h>

enum{
    LOGLEVEL_DEBUG,
    LOGLEVEL_INFO,
    LOGLEVEL_WARN,
    LOGLEVEL_ERROR,
};

#define __log_level LOGLEVEL_DEBUG

#define _LOG(level,fp,fmt,...) if(LOGLEVEL_##level>=__log_level) fprintf(fp,fmt "\n",##__VA_ARGS__)
#define LOGDEBUG(...) _LOG(DEBUG,stdout,__VA_ARGS__)
#define LOGINFO(...) _LOG(INFO,stdout,__VA_ARGS__)
#define LOGWARN(...) _LOG(WARN,stdout,__VA_ARGS__)
#define LOGERROR(...) _LOG(ERROR,stdout,__VA_ARGS__)

#endif //C_PLAYGROUND_LOG_H
