#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

int
main(int ac, char **av)
{
  long long time = 0;
  if (ac >= 2) {
    time = atoll(av[1]);
  }

  printf("starting\r\n");
  while (1) {
    printf("sleep: ");
    struct timeval start, end;
    gettimeofday(&start, NULL);
    int rv = usleep(time);
    gettimeofday(&end, NULL);
    long long diff = (end.tv_sec * 1000000 + end.tv_usec) -
      (start.tv_sec * 1000000 + start.tv_usec) ;
    printf("rv = %d, time = %lld, errno = %s\r\n", rv, diff, strerror(errno));
  }
  return 0;
}
