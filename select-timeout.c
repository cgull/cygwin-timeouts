#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#include <sys/select.h>
#include <time.h>
#include <signal.h>

struct termios saved_tattr;

void
restore_tattr (void)
{
  if (isatty (STDIN_FILENO))
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_tattr);
}

int
ready_select ()
{
  struct timeval timeoutstru;
  fd_set readfds;
  int nfds;
  static int rv = 0;

  timeoutstru.tv_sec = 0;
  timeoutstru.tv_usec = 800000;

  FD_ZERO (& readfds);
  FD_SET (STDIN_FILENO, & readfds);
  nfds = select (STDIN_FILENO + 1, & readfds, 0, 0, & timeoutstru);
  printf ("(%d)", nfds);
  return nfds;
}

int
main (void)
{
  if (isatty (STDIN_FILENO)) {
    struct termios tattr;
    tcgetattr (STDIN_FILENO, &saved_tattr);
    tattr = saved_tattr;
/*
    tattr.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
    tattr.c_oflag &= ~OPOST;
    tattr.c_cflag &= ~(CSIZE|PARENB);
    tattr.c_cflag |= CS8;
    tattr.c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
*/
    /* use miminal change for test case: */
    tattr.c_lflag &= ~(ICANON);
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
    atexit (restore_tattr);
  }
  setbuf (stdout, 0);

  printf ("\033[?1000h\033[?1006h\033[?1004h");
  while (1) {
    unsigned char c;
    int res;

    while (! ready_select ()) {
    }

    res = read (STDIN_FILENO, & c, 1);
    switch (c & 0xFF) {
      case 0x00 ... 0x1f:  putchar ('^'); putchar (c + 0x40);
                           if (c == '\r') printf ("\r\n");
                           break;
      case 0x7f: putchar ('^'); putchar ('?'); break;
      case 0x80 ... 0xFF: printf ("\\x%2X", c); break;
      default: putchar (c);
    }
    fflush (stdout);
  }
  printf ("\033[?1000l\033[?1004l\r\nbye\r\n");
  return 0;
}
