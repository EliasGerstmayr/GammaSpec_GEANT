%% convert mat to csv files

source = './matfiles/';
target = './csvfiles/';

matlist = dir([source '*.mat']);
matlist = {matlist.name}';

for i=1:length(matlist)
    load([source matlist{i}])
    
    x = Y_MEVlin(Y_MEVlin<= manual_cutoff_MEV)';
    y = spectrum_integrated_lin(Y_MEVlin<= manual_cutoff_MEV);
    figure;
    plot(x,y)
    A = [x y];
    output_filename = [target matlist{i}(1:(end-4-6)) '.dat'];
    dlmwrite(output_filename, A, ' ');
end

