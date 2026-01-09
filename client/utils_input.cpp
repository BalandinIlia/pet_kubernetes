#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

bool ready()
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    struct timeval tv;
    tv.tv_sec=0;
    tv.tv_usec=0;
    int ret = select(STDIN_FILENO+1, &set, NULL, NULL, &tv);
    return (ret>0);
}
