#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>

void
select0()
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  select(0, NULL, NULL, NULL, &tv);
}

void
select1()
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 1;
  select(0, NULL, NULL, NULL, &tv);
}
void
sleep0()
{
  sleep(0);
}
void
sleep1()
{
  sleep(1);
}
void
usleep0()
{
  usleep(0);
}
void
usleep1()
{
  usleep(1);
}
void
nanosleep0()
{
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 0;
  nanosleep(&ts, NULL);
}
void
nanosleep1()
{
  struct timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 1;
  nanosleep(&ts, NULL);
}
  
#define E(x) { x, #x },

struct test { void(*f)(); char *n; };

struct test tests[] = {
  E(select0)
  E(select1)
  E(sleep0)
  E(sleep1)
  E(usleep0)
  E(usleep1)
  E(nanosleep0)
  E(nanosleep1)
  { NULL, NULL }
};

int
main(int ac, char **av)
{
  (void)ac;
  (void)av;
  const struct test *p = tests;
  while (p->f) {
    printf("calling %s: ", p->n);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    p->f();
    gettimeofday(&end, NULL);
    long long diff = (end.tv_sec * 1000000 + end.tv_usec) -
      (start.tv_sec * 1000000 + start.tv_usec) ;
    printf("time %lld\n", diff);
    p++;
  }
  return 0;
}
