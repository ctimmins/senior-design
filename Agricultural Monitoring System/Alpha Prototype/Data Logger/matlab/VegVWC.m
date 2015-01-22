function [ vwc ] = VegVWC( vin )
%VEGVWC Converts vegetronix analog voltage output to vwc percentage
%   Detailed explanation goes here
    
    if (vin < 1.1)
        vwc = 10*vin - 1;
    elseif (vin < 1.3) && (vin >= 1.1)
        vwc = 25*v_in - 17.5
    elseif (vin >= 1.3) && (vin < 1.82)
        vwc = 48.08*vin - 47.5
    elseif (vin >= 1.82) && (vin < 2.2)
        vwc = 26.32*vin - 7.89
    

end

