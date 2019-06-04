clear
listing = dir('*.mat');


for n = 1:length(listing)
    filenames{n} = listing(n).name;
    load(listing(n).name)
%     figure(1)
%     subplot(1,length(listing),n)
%     imagesc(CsIhist')
%     edep = sum(CsIhist(:))/1e3;
%     title(['Deposited ' num2str(round(100*edep)/100) ' GeV'])
%     axis image xy
%     caxis([0 1000])
%     spectral
    det(:,n) = sum(CsIhist');
    detcent(:,n) = CsIhist(:, 25);
end

figure(2)
load('/Users/jmc208/Documents/MATLAB/Gemini Mangles 2015/Simplified_Iterative/simData.mat')
h = gca;
    plot(det/6.7e4)
xlabel('Column')
ylabel('MeV per column')
%leg = legend(filenames);
MakeSeabornPlot
xlim([1 33])