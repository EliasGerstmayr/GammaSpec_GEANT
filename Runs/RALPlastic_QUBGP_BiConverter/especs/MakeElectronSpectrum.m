x = dlmread('./csvfiles/Y_MEVlin.dat');
y = dlmread('./csvfiles/spectrum_integrated_lin.dat');
%I = importdata('./Null/20151217r002s015_espec2D.dat');
figure;
%subplot(211)
plot(x,y)
%subplot(212)
%imagesc(I)

%%

%A = [x(1:1024)' y(1:1024)'./x(1:1024)'];
A = [x y];
dlmwrite('gemElectronSpectrum.dat', A, ' ');