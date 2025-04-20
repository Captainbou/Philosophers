#include <sys/time.h>
#include <stdio.h>
#include <libc.h>
int main()
{
    struct timeval tv;
    memset(&tv, 0, sizeof(tv));
    gettimeofday(&tv, NULL);
    printf("%ld\n", tv.tv_sec);
}