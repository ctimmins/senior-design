function [ handle, x, y, z ] = readXYZ( XBee, handles )
%READXYZ Summary of this function goes here
%   Detailed explanation goes here
    xh = fread(XBee, 1, 'int8')*256;
    xl = fread(XBee, 1, 'uint8');
    x = xh + xl;

    yh = fread(XBee, 1, 'int8')*256;
    yl = fread(XBee, 1, 'uint8');
    y = yh + yl;

    zh = fread(XBee, 1, 'int8')*256;
    zl = fread(XBee, 1, 'uint8');
    z = zh + zl;
    
    %tmp1 = handles.XYZ(1,:);
    tmp2 = handles.XYZ(2,:);
    tmp3 = handles.XYZ(3,:);
    handles.XYZ = [tmp2; tmp3; [x y z]];
    handle = handles;
    bar3(handles.signalAxes, handles.XYZ);
end

