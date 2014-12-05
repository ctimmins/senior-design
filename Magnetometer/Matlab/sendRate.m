function [ output_args ] = sendRate( hObject, eventdata,handles )
%SENDCOMMAND Summary of this function goes here
%   Detailed explanation goes here
    XBee = handles.XBee;
    adcRate = cellstr(get(handles.adcRateDrpDwn, 'String'));
    adcRate = adcRate{get(handles.adcRateDrpDwn, 'Value')};
    outRate = cellstr(get(handles.outRateDrpDwn, 'String'));
    outRate = outRate{get(handles.outRateDrpDwn, 'Value')};
    osRatio = cellstr(get(handles.osRatioDrpDwn, 'String'));
    osRatio = osRatio{get(handles.osRatioDrpDwn, 'Value')};
    %startRange = 2;
%     adcRate = get(handles.adcRateDrpDwn, 'String');
%     outRate = get(handles.outRateDrpDwn, 'String');
%     osRatio = get(handles.osRatioDrpDwn, 'String');
    numADC = str2double(adcRate);
    numODR = str2double(outRate);
	numOSR = str2double(osRatio);
	handles.sampRateBuffer = [numADC numODR numOSR];
    
    
    switch adcRate
        case '1280'
            switch osRatio
                case '16'
                    fwrite(XBee, 2, 'uint8');
                case '32'
                    fwrite(XBee, 3, 'uint8');
                case '64'
                    fwrite(XBee, 4, 'uint8');
                case '128'
                    fwrite(XBee, 5, 'uint8');
                otherwise
                    warndlg('Invalid selection');
            end
                
        case '640'
            switch osRatio
                case '16'
                    fwrite(XBee, 6, 'uint8');
                case '32'
                    fwrite(XBee, 7, 'uint8');
                case '64'
                    fwrite(XBee, 8, 'uint8');
                case '128'
                    fwrite(XBee, 9, 'uint8');
                otherwise
                    warndlg('Invalid selection');
            end
                
        case '320'
            switch osRatio
                case '16'
                    fwrite(XBee, 10, 'uint8');
                case '32'
                    fwrite(XBee, 11, 'uint8');
                case '64'
                    fwrite(XBee, 12, 'uint8');
                case '128'
                    fwrite(XBee, 13, 'uint8');
                otherwise
                    warndlg('Invalid selection');
            end
                
        case '160'
            switch osRatio
                case '16'
                    fwrite(XBee, 14, 'uint8');
                case '32'
                    fwrite(XBee, 15, 'uint8');
                case '64'
                    fwrite(XBee, 16, 'uint8');
                case '128'
                    fwrite(XBee, 17, 'uint8');
                otherwise
                    warndlg('Invalid selection');
            end
            
        case '80'
            switch osRatio
                case '16'
                    switch outRate
                        case '5.00'
                            fwrite(XBee, 18, 'uint8');
                        case '2.50'
                            fwrite(XBee, 22, 'uint8');
                        case '1.25'
                            fwrite(XBee, 26, 'uint8');
                        case '0.63'
                            fwrite(XBee, 30, 'uint8');
                        otherwise
                            warndlg('Invalid selection');
                    end
                    
                case '32'
                    switch outRate
                        case '2.50'
                            fwrite(XBee, 19, 'uint8');
                        case '1.25'
                            fwrite(XBee, 23, 'uint8');
                        case '0.63'
                            fwrite(XBee, 27, 'uint8');
                        case '0.31'
                            fwrite(XBee, 31, 'uint8');
                        otherwise
                            warndlg('Invalid selection');
                    end
                case '64'
                    switch outRate
                        case '1.25'
                            fwrite(XBee, 20, 'uint8');
                        case '0.63'
                            fwrite(XBee, 24, 'uint8');
                        case '0.31'
                            fwrite(XBee, 28, 'uint8');
                        case '0.16'
                            fwrite(XBee, 32, 'uint8');
                        otherwise
                            warndlg('Invalid selection');
                    end
                case '128'
                    switch outRate
                        case '0.63'
                            fwrite(XBee, 21, 'uint8');
                        case '0.31'
                            fwrite(XBee, 25, 'uint8');
                        case '0.16'
                            fwrite(XBee, 29, 'uint8');
                        case '0.08'
                            fwrite(XBee, 33, 'uint8');
                        otherwise
                            warndlg('Invalid selection');
                    end
                otherwise
                    warndlg('Invalid selection');
            end
        otherwise
            warndlg('Invalid Selection')
    end

end
