function [ s ] = setUpComm( port,  )
%SETUPCOMM Summary of this function goes here
%   Detailed explanation goes here
    %s = serial(port, 'BaudRate', 9600);
    fclose(instrfind);
    s.BytesAvailableFcnCount = 1;
    s.BytesAvailableFcnMode = 'byte';
    s.BytesAvailableFcn = @byteAvailableInt;

    fopen(s);         
    fwrite(s, 123);
    
    %wait for xbee to send command back
    %readasync(s);
    
    %while s.BytesAvailable == 0
    %end
    response = fread(s,1,'uint8');
    
   % response = readData8(s);
    if response == 123
        disp('Communication Established Successfully');
    else
        disp('Something Went Wrong');
        
       
    
end

