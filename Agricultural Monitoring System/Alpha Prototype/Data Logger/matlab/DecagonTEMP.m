function [ t ] = DecagonTEMP( t_raw )
%Converts Decagon DDI ascii stream to actual temperature
%   Detailed explanation goes here
    t = (t_raw - 400) / 10;

end

