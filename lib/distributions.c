#include "distributions.h"
#include <math.h>
#include <stdlib.h>

double nextService(double dm){
    double u = _random();
	return -(double)log(u)*dm;
}
double nextArrival(double l, int freePopulation){
    double u = _random();
	double l2 = l*freePopulation;
	return -(double)log(u)/l2;
}
double box_muller(double mu,double sigma){
    double u1 = _random();
    double u2 = _random();
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0*M_PI*u2);
    //double z1 = sqrt(-2.0 * log(u1)) * sin(2.0*M_PI*u2);
    return z0*sigma + mu;
}
double _random(){
    return ((double)rand() + 1.0)/(double)(RAND_MAX + 1.0);
}

double nextServiceInterval(double dm,double min, double max){
    double r = min - 1;
    while (r < min || r > max)
        r = nextService(dm);
    return r;
}
double nextBoxInterval(double mu, double sigma,double min, double max){
    double r = min - 1;
    while (r < min || r > max)
        r = box_muller(mu,sigma);        
    return r;
}