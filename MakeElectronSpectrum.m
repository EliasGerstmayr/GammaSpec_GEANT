x = importdata('X_MEVlin.dat');
y = importdata('./Null/20151217r002s015_especint.dat');
I = importdata('./Null/20151217r002s015_espec2D.dat');
subplot(211)
plot(x,y./x)
subplot(212)
imagesc(I)

%%

A = [x(1:1024)' y(1:1024)'./x(1:1024)'];
dlmwrite('~/GEANT/B1/gemElectronSpectrum.dat', A, ' ');