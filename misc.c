#include "misc.h"

#include <stdio.h>
#include <unistd.h>

void closefp(void*arg){
    FILE*fp=*(FILE**)arg;
    if(fp)
        fclose(fp);
}

void closefd(void*arg){
    int fd=*(int*)arg;
    if(fd!=-1)
        close(fd);
}