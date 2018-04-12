#define OUTPUT "csv"
#define BINSIZE 1

typedef struct _histo{
    int *bins;
    int maxValue;
    int totalCount;
    int size;
    int minValue;
    int offset;
    
    double sum;
} histogram;

histogram *newHistogram(int maxSize);
void deleteHistogram(histogram *hist);
void addHistogramEntry(histogram *hist,double newValue);
void saveHistogram(const char* file,histogram *hist);
void resetHist(histogram *hist);