#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __CYGWIN__
#include <cygwin/time.h>
#endif
#include <sys/time.h>

int
main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "no args\n");
    exit(1);
  }
  struct timespec ts;
  char *ep;
  ts.tv_sec = 0;
  ts.tv_nsec = strtol(argv[1], &ep, 0);
  if (*argv[1] != '\0' && *ep != '\0') {
    fprintf(stderr, "bad resolution value\n");
    exit(1);
  }
#ifdef __CYGWIN__
  clock_setres(CLOCK_REALTIME, &ts);
#endif
  if (argc > 2) {
    execvp(argv[2], argv+2);
    return errno;
  } else {
    pause();
  }
  return 0;
}
	       
