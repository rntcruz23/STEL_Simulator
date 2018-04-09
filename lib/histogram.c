#include "histogram.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

histogram *newHistogram(int maxSize){
    histogram *new = (histogram *) calloc(sizeof(histogram),1);
    new->bins = (int *)calloc(sizeof(int),maxSize);
    return new;
}
void deleteHistogram(histogram *hist){
    free(hist->bins);
    free(hist);
}
void addHistogramEntry(histogram *hist,double newValue){
    int bin = (int)newValue/BINSIZE;
    hist->maxValue = hist->maxValue>bin?hist->maxValue:bin;
    hist->bins[bin]++;
    hist->totalCount++;
    hist->sum += newValue;
}
void saveHistogram(const char* file,histogram *hist){
    int fd = open(file,O_CREAT|O_WRONLY|O_TRUNC,S_IWUSR|S_IRUSR);
	int i = 0;
	char new_val[100] = {0};
	sprintf(new_val,"%d",hist->bins[0]);
	write(fd,new_val,strlen(new_val));
	for(i = 1; i < hist->maxValue;i++){
		sprintf(new_val,",%d",hist->bins[i]);
		write(fd,new_val,strlen(new_val));
	}
    close(fd);
}