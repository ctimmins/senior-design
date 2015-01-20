function [ vwc ] = DecagonVWC( raw_d )
%DecagonVWC Calculates VWC given raw dielectric input
%  inputs:   raw_d  raw dielectric output
%  outputs:  vwc    volumetric water content (m^3/m^3)
    
    d = raw_d / 50;
    vwc = ((4.3e-6)*d^3) - ((5.5e-4)*d^2) + ((2.92e-2)*d) - (5.3e-2);
    
end

