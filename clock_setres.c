#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cygwin/time.h>
#include <sys/time.h>

int
main(int argc, char **argv)
{

  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = atoi(argv[1]);
  clock_setres(CLOCK_REALTIME, &ts);
  if (argc > 2) {
    execvp(argv[2], &argv[3]);
    return errno;
  } else {
    pause();
  }
  return 0;
}
	       
