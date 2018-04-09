close all;

civilhist = csvread('csv/service0.csv');
inemhist = csvread('csv/service1.csv');
prediction = csvread('csv/predict0.csv');

bins = 5;

ax = (0:size(civilhist,2)-1)*bins;
ax1 = (0:size(inemhist,2)-1)*bins;
ax2 = (0:size(prediction,2)-1)*bins;

figure; plot(ax,civilhist);
figure; plot(ax1,inemhist);
figure; plot(ax2,prediction);
