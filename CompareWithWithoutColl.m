clear
clf
load('~/GEANT/QED/data/WithColl.mat')

ebins =  linspace(0, 2000, 100);
rad = 50;

[N,X] = hist(TrackerGamma.e(sqrt(TrackerGamma.x.^2 + TrackerGamma.y.^2) < rad),ebins);
%plot(X,N.*X)

[N,X] = hist3([TrackerGamma.x, TrackerGamma.y], {[-200:2:200], [-200:2:200]});
subplot(121)
imagesc(N)
axis image xy
caxis([0 50])
%plot(sum(N))


load('~/GEANT/QED/data/WithoutColl.mat')

[N,X] = hist(TrackerGamma.e(sqrt(TrackerGamma.x.^2 + TrackerGamma.y.^2) < rad),ebins);
hold on
%plot(X,N.*X)
hold off

xlabel('x /mm')
ylabel('y /mm')
title('With collimator')
MakeSeabornPlot();
grid off

[N,X] = hist3([TrackerGamma.x, TrackerGamma.y], {[-200:2:200], [-200:2:200]});
subplot(122)
imagesc(N)
axis image xy
caxis([0 50])
%hold on
%plot(sum(N))
%hold off

% set(gca, 'YScale', 'log')

xlabel('x /mm')
ylabel('y /mm')
%leg = legend('With collimator', 'Without collimator');

title('Without collimator')
MakeSeabornPlot();
grid off


spectral