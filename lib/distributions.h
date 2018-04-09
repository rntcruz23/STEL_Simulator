#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


double nextServiceInterval(double dm,double min, double max);
double nextBoxInterval(double mu, double sigma,double min, double max);
double nextArrival(double l, int freePopulation);
double _random();