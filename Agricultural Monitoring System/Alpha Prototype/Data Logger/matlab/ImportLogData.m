function [ M ] = ImportLogData( fn )
%IMPORTLOGDATA Imports the text file in data folder for further processing
% inputs    fn   path to filename      
% outputs   M    Cell Matrix holding all log data  

    f = fopen(fn);
    M = textscan(f, '%10s', 'Delimiter', '\r\n');
end

