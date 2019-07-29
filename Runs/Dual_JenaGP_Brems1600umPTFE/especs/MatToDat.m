%% convert mat to csv files

source = './matfiles/';
target = './csvfiles/';

matlist = dir([source '*.mat']);
matlist = {matlist.name}';

threshold = 1500;

for i=1:length(matlist)
    load([source matlist{i}])
    
    x = W_MeV(W_MeV<threshold);
    y = E_spec_lineout(W_MeV<threshold)';
    figure;
    plot(x,y)
    A = [x y];
    output_filename = [target matlist{i}(1:(end-4-6)) '.dat'];
    dlmwrite(output_filename, A, ' ');
end

