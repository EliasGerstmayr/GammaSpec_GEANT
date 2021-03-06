%variable = linspace(0, 1, 20);
variable = 10:10:900;

% Based on the input decks runGamma
%executecommand = '';
executecommand = strings(length(variable),1);

for n = 1:length(variable)
    var = variable(n);
    macfname = ['/Users/eg415/GEANT/QED/scripts_mono/Gamma_' num2str(var, '%03d') 'MeV.mac'];
    outputfname = ['Gamma_' num2str(var, '%03d') 'MeV'];

    inputdeck= ['/run/numberOfThreads 1' char(10),...
                '/run/initialize' char(10), ...
                '/vis/open HepRepFile' char(10), ...
                '/vis/drawVolume' char(10), ...
                '/vis/scene/add/trajectories smooth' char(10), ...
                '/vis/scene/endOfEventAction accumulate 10' char(10), ...
                '/gps/particle gamma' char(10), ...
                '/gps/pos/centre 0 0 42.5 cm' char(10), ...
                '/gps/ang/type beam1d' char(10), ...
                '/gps/ang/sigma_r 0.02 deg' char(10), ...
                '/gps/pos/type Beam' char(10), ...
                '/gps/pos/shape Circle' char(10), ...
                '/gps/pos/sigma_r 0.425 um' char(10), ...
                '/gps/ene/type Lin' char(10), ...
                '/gps/ene/min ' num2str(var) ' MeV' char(10), ...
                '/gps/ene/max ' num2str(var) ' MeV' char(10), ...
                '/gps/ene/gradient 0' char(10), ...
                '/gps/ene/intercept 1' char(10), ...
                '/B1/histo/setOutputFile ' outputfname char(10), ...
                '/run/beamOn 10000' char(10),...
                '/vis/viewer/flush'];

    fid = fopen(macfname, 'w');
    fprintf(fid, inputdeck);
    fclose(fid);
    executecommand{n} =  ['./QED ' macfname ';'];
    %executecommand = [executecommand './QED ' macfname '; python root2mat.py ./data/' outputfname '.root; '];

end

executecommand_file = 'buffer_1.txt';
fid = fopen(executecommand_file, 'w');
fprintf(fid,'%s\n', executecommand);
fclose(fid);
%clipboard('copy',executecommand)
