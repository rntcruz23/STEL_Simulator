#include "histogram.h"

#define WINDOW  (1000)

typedef struct _predict{
    double prediction;          /* Current prediction for queue time    */
    double correct;             /* Current queue time                   */
    histogram *h;               /* Error histogram                     */
} prediction;

prediction *newPredictModel();
void deletePredictions(prediction *predict);
void addPredictionEntry(prediction *predict,double newValue);
void addCorrectEntry(prediction *predict,double correctValue);
double getCurrentPrediction(prediction *predict);