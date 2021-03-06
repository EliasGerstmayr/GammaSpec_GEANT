%variable = linspace(0, 1, 20);
variable = 500:500:200000;

% Based on the input decks runGamma
%executecommand = '';
executecommand = strings(length(variable),1);

for n = 1:length(variable)
    var = variable(n);
    macfname = ['/Users/eg415/GEANT/QED/scripts_gamma_mono_novis/GammaNovis_' num2str(var, '%06d') 'keV.mac'];
    outputfname = ['GammaNovis_' num2str(var, '%06d') 'keV'];

    inputdeck= ['/run/numberOfThreads 1' char(10),...
                '/run/initialize' char(10), ...
                '/vis/disable' char(10), ...
                '/gps/particle gamma' char(10), ...
                '/gps/pos/centre 0 0 42.5 cm' char(10), ...
                '/gps/ang/type beam1d' char(10), ...
                '/gps/ang/sigma_r 0.0 deg' char(10), ... %0.02 deg
                '/gps/pos/type Beam' char(10), ...
                '/gps/pos/shape Circle' char(10), ...
                '/gps/pos/sigma_r 0.0 um' char(10), ... %0.425
                '/gps/ene/type Lin' char(10), ...
                '/gps/ene/min ' num2str(var) ' keV' char(10), ...
                '/gps/ene/max ' num2str(var) ' keV' char(10), ...
                '/gps/ene/gradient 0' char(10), ...
                '/gps/ene/intercept 1' char(10), ...
                '/B1/histo/setOutputFile ' outputfname char(10), ...
                '/run/beamOn 100000' char(10)];

    fid = fopen(macfname, 'w');
    fprintf(fid, inputdeck);
    fclose(fid);
    executecommand{n} =  ['./QED ' macfname ';'];
    %executecommand = [executecommand './QED ' macfname '; python root2mat.py ./data/' outputfname '.root; '];

end

executecommand_file = 'buffernovis_2.txt';
fid = fopen(executecommand_file, 'w');
fprintf(fid,'%s\n', executecommand);
fclose(fid);
%clipboard('copy',executecommand)
