%variable = linspace(0, 1, 20);
variable = 1;

executecommand = '';

for n = 1:length(variable)
    var = variable(n);
    macfname = ['/Users/eg415/GEANT/QED/Gamma_' num2str(var, '%03d') '.mac'];
    outputfname = ['Gamma_' num2str(var, '%05d')];

    inputdeck= ['/run/initialize' char(10), ...
                '/gps/particle gamma' char(10), ...
                '/gps/ene/type Gauss' char(10), ...
                '/gps/ene/mono 50 MeV' char(10), ...
                '/gps/ene/sigma 0.001 MeV' char(10), ...
                '/gps/ang/type beam1d' char(10), ...
                '/gps/ang/sigma_r ' num2str(var) ' deg' char(10), ...
                '/gps/pos/type Point' char(10), ...
                '/gps/position 0 0 50 cm' char(10), ...
                '/B1/histo/setOutputFile ' outputfname char(10), ...
                '/run/beamOn 10000'];

    fid = fopen(macfname, 'w');
    fprintf(fid, inputdeck);
    fclose(fid);

    executecommand = [executecommand './QED ' macfname '; python root2mat.py ./data/' outputfname '.root; '];

end

clipboard('copy',executecommand)