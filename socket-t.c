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
  long long time = -1;
  if (ac >= 2) {
    time = atoll(av[1]);
  }
  unsigned int count = UINT_MAX;
  if (ac >= 3) {
    count = atoi(av[2]);
  }
  if (setvbuf(stdin, NULL, _IONBF, 0) != 0) {
    err(1, "stdin setvbuf");
  }
  if (setvbuf(stdout, NULL, _IONBF, 0) != 0) {
    err(1, "stdout setvbuf");
  }
  struct termios o, n;
  tcgetattr(0, &o);
  tcgetattr(0, &n);
  cfmakeraw(&n);
  tcsetattr(0, TCSADRAIN, &n);

  printf("starting\r\n");
  while (1) {
    while (1) {
      if (!count--) {
	goto out;
      }
      fd_set reads, errors;
      FD_ZERO(&reads);
      FD_ZERO(&errors);
      FD_SET(0, &reads);
      FD_SET(0, &errors);
      struct timeval tv, *tvp = NULL;
      tv.tv_sec = time / 1000000;
      tv.tv_usec = time % 1000000;
      if (time >= 0) {
	tvp = &tv;
      }
      printf("select: ");
      struct timeval start, end;
      gettimeofday(&start, NULL);
      int rv = select(1, &reads, NULL, &errors, tvp);
      gettimeofday(&end, NULL);
      long long diff = (end.tv_sec * 1000000 + end.tv_usec) -
	(start.tv_sec * 1000000 + start.tv_usec) ;
      printf("rv = %d, time = %lld, errno = %s\r\n", rv, diff, strerror(errno));
      if (rv == 0) {
	if (FD_ISSET(0, &reads)) printf ("unexpected read\r\n");
	if (FD_ISSET(0, &errors)) printf ("unexpected error\r\n");
	if (FD_ISSET(0, &reads) || FD_ISSET(0, &errors)) break;
	// otherwise continue
      } else if (rv == 1) {
	if (FD_ISSET(0, &reads)) {
	// select found something normally, go read it
	  break;
	} else if (FD_ISSET(0, &errors)) {
	  printf("error on stdin\r\n");
	  break;
	} else {
	  printf("no ready fd found\r\n");
	  break;
	}
      } else if (rv > 1) {
	printf("unexpected count = %d\r\n", rv);
	break;
      } else {
	printf("rv %d, error %s\r\n", rv, strerror(errno));
	break;
      }
    }
    printf("reading char: ");
    char c = getc(stdin);
    if (c == 0x7f) break;
    printf("got 0x%02x\r\n", c);
  }
 out:
  tcsetattr(0, TCSADRAIN, &o);
  return 0;
}
