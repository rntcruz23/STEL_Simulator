close all;

civilhist = csvread('csv/service0.csv');
inemhist = csvread('csv/service1.csv');
civilprediction = csvread('csv/predict0.csv');
inemprediction = csvread('csv/predict1.csv');
gaussian = csvread('csv/gaussian.csv');

bins = 3;
minbin = size(gaussian,2)/2;
axis = -minbin:minbin-1;

figure; plot(axis,civilhist);
figure; plot(axis,inemhist);
figure; plot(axis,civilprediction);
figure; plot(axis,inemprediction);
figure; plot(axis,gaussian);
