#include <sys/time.h>
#include <stdio.h>

static inline double ms()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);

  double ms = (double)(tp.tv_sec)*1000 + (double)(tp.tv_usec)/1000;
  return ms;
}

static inline double ms_toc(double* start)
{
  double now = ms();
  double dt = now - *start;
  *start = now;
  printf("toc: %f\n", dt);
  return dt;
}

//static inline double ms_toc_avg(double* start)
//{
//  double now = ms();
//  double dt = now - *start;
//  *start = now;
//
//  static double sum=0;
//  static long long unsigned int n=0;
//  sum += (double)dt;
//  ++n;
//  double avg = sum / (double)n;
//  printf("toc: %f\t%llu\n", avg, n);
//  return avg;
//}
