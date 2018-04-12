#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "predictions.h"

prediction *newPredictModel(){
    prediction *new = (prediction *)calloc(sizeof(prediction),1);
    new->h = newHistogram(10000);
    return new;
}
void deletePredictions(prediction *predict){
    deleteHistogram(predict->h);
    free(predict);
}
void addCorrectEntry(prediction *predict,double correctValue){
    predict->correct = predict->correct - (double)predict->correct/WINDOW + (double)correctValue/WINDOW;
    addHistogramEntry(predict->h,predict->prediction-predict->correct);
}
void addPredictionEntry(prediction *predict,double newValue){
    predict->prediction = predict->prediction - (double)predict->prediction/WINDOW + (double)newValue/WINDOW;
    if(predict->correct)       /*  Only add error if at least one delayed call finished    */
        addHistogramEntry(predict->h,predict->prediction-predict->correct);
}
double getCurrentPrediction(prediction *predict){
    return predict->correct;
}