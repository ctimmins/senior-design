function varargout = magGUI(varargin)
% MAGGUI MATLAB code for magGUI.fig
%      MAGGUI, by itself, creates a new MAGGUI or raises the existing
%      singleton*.
%
%      H = MAGGUI returns the handle to a new MAGGUI or the handle to
%      the existing singleton*.
%
%      MAGGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MAGGUI.M with the given input arguments.
%
%      MAGGUI('Property','Value',...) creates a new MAGGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before magGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to magGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help magGUI

% Last Modified by GUIDE v2.5 05-Dec-2014 05:14:40

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @magGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @magGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
end

% --- Executes just before magGUI is made visible.
function magGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to magGUI (see VARARGIN)

% Choose default command line output for magGUI
handles.output = hObject;
%ADDED KEITH
XYZ_Array = [0 0 0;0 0 0; 0 0 0];
handles.XYZ = XYZ_Array;
offsetValues = [0 0 0 0 0 0];
handles.offsetValues = offsetValues;
offsetFlag = 0;
handles.offsetFlag = offsetFlag;
data = zeros(11,3);
set(handles.uitable,'Data',data);
sampRateBuffer = [0 0 0];
handles.sampRateBuffer = sampRateBuffer;
% Update handles structure
guidata(hObject, handles);


%-----------------------------------------------------------------%
%---------------- Initialize Xbee serial port --------------------%
%-----------------------------------------------------------------%
portName = get(handles.serialPortNameBox, 'String');
XBee = serial(portName, 'BaudRate', 9600);


%trigger callback when there is an incoming byte
XBee.BytesAvailableFcnCount = 1;
XBee.BytesAvailableFcnMode = 'byte';
XBee.BytesAvailableFcn = @(obj,event)byteAvailableInt(obj,event,hObject);

%init this shit
fclose(instrfind);
%fwrite(XBee, 123);
%response = fread(XBee, 1, 'uint8');
%if response == 123
%    disp('Communication Established Successfully');
%else
%    disp('Something Went Wrong');
%end

%-----------------------------------------------------------------%
%------------------ Declare Global Variables ---------------------%
%-----------------------------------------------------------------%
handles.isActive = false;
handles.XBee = XBee;
handles.portName = portName;



guidata(hObject, handles);

%-----------------------------------------------------------------%
%---------------- Initialize Bar Graph for XYZ --------------------%
%-----------------------------------------------------------------%
%set(handles.signalAxes, 'Title', 'XYZ Magnetic Field Density');
%set(handles.signalAxes, 'XLabel', 'X (Tesla)');
%set(handles.signalAxes, 'YLabel', 'Y (Tesla)');
%set(handles.signalAxes, 'ZLabel', 'Z (Tesla)');
handles.hg = bar3(handles.signalAxes, handles.XYZ,'grouped');
%set(handles.signalAxes,'XTickLabel',['O';'L';'N']);
set(handles.signalAxes,'YTickLabel',['O';'L';'N']);
tick_size = 0.2;
num_elements = 2/tick_size;
ztickArray = zeros(1,num_elements);
ztickArray(1) = 0;

for i = 0:1:num_elements
    ztickArray(i+1) = -1+(tick_size*i);
end
%disp(ztickArray);
%set(handles.signalAxes,'ZTick', [0;.05;.1;.15;.2;.25;.3;.35;.4;.45;.5;.55;.6;.65;.7;.75;.8;.85;.9;.95;1]);
set(handles.signalAxes,'ZTick', ztickArray);
handles.ztickArray = ztickArray;

%-----------------------------------------------------------------%
%---------------- Initialize Buttons/TextBoxes --------------------%
%-----------------------------------------------------------------%
%set(hObject, 'String', 'Open Port');
set(handles.portConnectStatus, 'Value', 0);
set(handles.commandList, 'Enable', 'off');
set(handles.standbyBtn, 'Enable', 'off');
set(handles.srBtn, 'Enable', 'off');
set(handles.activeBtn, 'Enable', 'off');
set(handles.setRateBtn, 'Enable','off');
% Keith Edit
%set(handles.figure1, 'Interruptible', 'off'); %make figure callbacks NON-interruptible
set(handles.portConnectBtn, 'Interruptible', 'off'); %make port open/close callback NON-interruptible
set(handles.offsetBtn, 'Enable','off');
set(handles.standbyBtn, 'Value',1); %make sure initial mode is standby
set(handles.xOffset, 'Enable', 'off');
set(handles.yOffset, 'Enable', 'off');
set(handles.zOffset, 'Enable', 'off');
set(handles.osRatioDrpDwn, 'Enable', 'off');
set(handles.adcRateDrpDwn, 'Enable', 'off');
set(handles.outRateDrpDwn, 'Enable', 'off');


guidata(hObject, handles);
% UIWAIT makes magGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);
end

% --- Outputs from this function are returned to the command line.
function varargout = magGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
end


function commandText_Callback(hObject, eventdata, handles)
% hObject    handle to commandText (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of commandText as text
%        str2double(get(hObject,'String')) returns contents of commandText as a double
end

% --- Executes during object creation, after setting all properties.
function commandText_CreateFcn(hObject, eventdata, handles)
% hObject    handle to commandText (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
end

%-----------------------------------------------------------------%
%-------------------- User Issued Commands -----------------------%
%-----------------------------------------------------------------%

% --- Executes on button press in sendButton.
function sendButton_Callback(hObject, eventdata, handles)
% hObject    handle to sendButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    command = cellstr(get(handles.commandList, 'String'));
    command = command{get(handles.commandList, 'Value')};
    disp(command);
    switch command
        case 'Read XYZ Data'
            fwrite(handles.XBee, 34, 'uint8');
        case 'Read CtrlReg1'
            fwrite(handles.XBee, 35, 'uint8');
        case 'Read CtrlReg2'
            fwrite(handles.XBee, 36, 'uint8');
        case 'Read DataReady'
            fwrite(handles.XBee, 37, 'uint8');
        case 'Read SystemMode'
            fwrite(handles.XBee, 38, 'uint8');
        case 'Read MagTemp'
            fwrite(handles.XBee, 39, 'uint8');
        case 'Read WhoAmI'
            fwrite(handles.XBee, 40, 'uint8');
        case 'Set Default (CtrlReg1)'
            fwrite(handles.XBee, 41, 'uint8');
        case 'Enable Fast-Read'
            fwrite(handles.XBee, 46, 'uint8');
        case 'Disable Fast-Read'
            fwrite(handles.XBee, 47, 'uint8');
        case 'Set Default (Ctrl2Reg)'
            fwrite(handles.XBee, 48, 'uint8');
        case 'Enable Auto-Reset'
            fwrite(handles.XBee, 49, 'uint8');
        case 'Disable Auto-Reset'
            fwrite(handles.XBee, 50, 'uint8');
        case 'Enable User-Offset'
            fwrite(handles.XBee, 51, 'uint8');
        case 'Disable User-Offset'
            fwrite(handles.XBee, 52, 'uint8');
        case 'Run MagRefresh'
            fwrite(handles.XBee, 53, 'uint8');
        case 'Set Default (All)'
            fwrite(handles.XBee, 54, 'uint8');          
    end
        
end

% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over sendButton.
function sendButton_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to sendButton (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end


% --- Executes on selection change in commandList.
function commandList_Callback(hObject, eventdata, handles)
% hObject    handle to commandList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns commandList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from commandList
end

% --- Executes during object creation, after setting all properties.
function commandList_CreateFcn(hObject, eventdata, handles)
% hObject    handle to commandList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
end


% --- Executes when selected object is changed in modeBtnGroup.
function modeBtnGroup_SelectionChangeFcn(hObject, eventdata, handles)
% hObject    handle to the selected object in modeBtnGroup 
% eventdata  structure with the following fields (see UIBUTTONGROUP)
%	EventName: string 'SelectionChanged' (read only)
%	OldValue: handle of the previously selected object or empty if none was selected
%	NewValue: handle of the currently selected object
% handles    structure with handles and user data (see GUIDATA)
    val = get(hObject, 'String');
    switch val
        case 'Standby'
            fwrite(handles.XBee, 42, 'uint8');
            disp('Standby Mode');
        case 'Active'
            fwrite(handles.XBee, 44, 'uint8');
            %Keith Edit
            set(handles.commandList, 'Enable', 'off');
            %set(handles.standbyBtn, 'Enable', 'off');
            set(handles.srBtn, 'Enable', 'off'); % single-read mode
            %set(handles.activeBtn, 'Enable', 'off');
            set(handles.setRateBtn, 'Enable','off');
            set(handles.offsetBtn, 'Enable','off');
            set(handles.xOffset, 'Enable', 'off');
            set(handles.yOffset, 'Enable', 'off');
            set(handles.zOffset, 'Enable', 'off');
            set(handles.osRatioDrpDwn, 'Enable', 'off');
            set(handles.adcRateDrpDwn, 'Enable', 'off');
            set(handles.outRateDrpDwn, 'Enable', 'off');   

            disp('Active Mode');
        case 'Single Read'
            fwrite(handles.XBee, 43, 'uint8');
            disp('Single Read Mode');
            % MUST BE RETURNED TO STANDBY MODE AFTER EXECUTION
        case 'Triggered'
            fwrite(handles.XBee, 45, 'uint8');
            disp('Triggered Mode');
            % MUST BE RETURNED TO ACTIVE MODE(?) AFTER EXECUTION
    end    
end


% --- Executes when user attempts to close figure1.
function figure1_CloseRequestFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


%temp_handles = guidata(handles); %import handles
%fwrite(temp_handles.XBee, 42, 'uint8'); %write MAG to standby before close
%set(temp_handles.standbyBtn, 'Value',1); %make sure at closing that mode is standby
%guidata(handles,temp_handles); %write back handles
%pause(1); %delay for 1 sec, to let write finish

%fwrite(handles.XBee, 42, 'uint8'); %write MAG to standby before close
%set(handles.standbyBtn, 'Value',1); %make sure at closing that mode is standby
%pause(1); %delay for 1 sec, to let write finish
try 
    fclose(handles.XBee);
catch
    disp('could not close port');
clear handles.XBee;
end

% Hint: delete(hObject) closes the figure
delete(hObject);
end

% --- Executes when there is a byte to be read in the buffer
function byteAvailableInt(obj, event,hObject)
    XBee = obj;
    temp_handles = guidata(hObject);
    %data = get(temp_handles.uitable,'Data');
    xposFR = 4;
    xposOFF = 5;
    xposDR = 9;
    xposWHO = 10;
    %xposSYS = 6;
    xposTEMP = 11;
    xposADC = 6;
    xposODR = 7;
    xposOSR = 8;
    
    while(XBee.BytesAvailable > 0)
        Command_Received = fread(XBee,1, 'uint8');
        
        switch Command_Received
            case 1 %Offset
                offset = temp_handles.offsetValues;
                fwrite(XBee,offset(1),'uint8');
                fwrite(XBee,offset(2),'uint8');
                fwrite(XBee,offset(3),'uint8');
                fwrite(XBee,offset(4),'uint8');
                fwrite(XBee,offset(5),'uint8');
                fwrite(XBee,offset(6),'uint8');
                temp_handles.offsetFlag = 0;
                guidata(hObject,temp_handles);
                    
                    

                
            case 34
                disp('XYZ VALUES:');
                %[x, y, z] = readXYZ(XBee)
                xh = fread(XBee, 1, 'int8')*256;
                xl = fread(XBee, 1, 'uint8');
                x = xh + xl;

                yh = fread(XBee, 1, 'int8')*256;
                yl = fread(XBee, 1, 'uint8');
                y = yh + yl;

                zh = fread(XBee, 1, 'int8')*256;
                zl = fread(XBee, 1, 'uint8');
                z = zh + zl;
                
                % convert from bit counts to [uT]
                x = x/10;
                y = y/10;
                z = z/10;
                data = get(temp_handles.uitable,'Data');
                data(1:3,3) = data(1:3,2);
                data(1:3,2) = data(1:3,1);
                data(1:3,1) = [x y z];
%                 data((3),(1)) = data((2),(1)); 
%                 data((3),(2)) = data((2),(2)); 
%                 data((3),(3)) = data((2),(3)); 
                
%                 data((2),(1)) = data((1),(1)); 
%                 data((2),(2)) = data((1),(2)); 
%                 data((2),(3)) = data((1),(3));

%                 data((1),(1)) = x; 
%                 data((1),(2)) = y; 
%                 data((1),(3)) = z; 
                set(temp_handles.uitable,'Data',data);
                
                c = temp_handles.XYZ;
                c(1,:) = temp_handles.XYZ(2,:);
                c(2,:) = temp_handles.XYZ(3,:);
                c(3,:) = [x y z];
                temp_handles.XYZ = c;   
                bar3(temp_handles.signalAxes,temp_handles.XYZ,'grouped');
                %set(temp_handles.signalAxes,'XTickLabel',['Oldest';'Last';'New']);
                set(temp_handles.signalAxes,'YTickLabel',['O';'L';'N']);
                guidata(hObject,temp_handles);
                
            case 35
                val = fread(XBee, 1, 'uint8');
                disp('Control Register 1:');
                disp(val);
                %% add update to the table values
                
            case 36
                val = fread(XBee, 1, 'uint8');
                disp('Control Register 2:');
                disp(val);
                if val>=16
                    data = get(temp_handles.uitable,'Data');                
                    data(xposOFF,1) = 1;                
                    set(temp_handles.uitable,'Data',data);  
                    guidata(hObject,temp_handles);
                else
                    data = get(temp_handles.uitable,'Data');                
                    data(xposOFF,1) = 0;                
                    set(temp_handles.uitable,'Data',data);  
                    guidata(hObject,temp_handles);                    
                end
                
            case 37
                val = fread(XBee, 1, 'uint8');
                disp('Data Ready Status Register:');
                
                disp(val);
                data = get(temp_handles.uitable,'Data');                
                data(xposDR,1) = val;                
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);                 
                warndlg('Data Ready Status Register Read Complete.');
            case 38
                val = fread(XBee, 1, 'uint8');
                disp('System Mode Register:');
                switch val
                    case 0
                        warndlg('Standby Mode');
                    case 1
                        warndlg('Single-Read Mode');
                    case 2
                        warndlg('Active Mode');
                    case 3
                        warndlg('Triggered Mode');
                    otherwise
                        warndlg('Unknown Mode');
                end
                disp(val);                
            case 39
                val = fread(XBee, 1, 'int8');
                data = get(temp_handles.uitable,'Data');                
                data(xposTEMP,1) = val;                
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);                
                disp('Die Temperature Register:');
                disp(val);   
                warndlg('Temperature Read Success');
            case 40
                val = fread(XBee, 1, 'uint8');
                
                disp('Who Am I Register:');
                data = get(temp_handles.uitable,'Data');                
                data(xposWHO,1) = val;                
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);                
                disp(val);
            case 41
                disp('Default (Ctrl1Reg) Write Successful');
                data = get(temp_handles.uitable,'Data');
                
                data(xposADC,1) = 0;
                data(xposODR,1) = 0;
                data(xposOSR,1) = 0;
                %data(xposSYS,1) = 0;
                set(temp_handles.activeBtn,'Value',0);
                set(temp_handles.standbyBtn,'Value',1);
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);
            case 42
                disp('Standby Mode Write Successful');
                           %Keith Edit
                set(temp_handles.commandList, 'Enable', 'on');
                %set(temp_handles.standbyBtn, 'Enable', 'off');
                set(temp_handles.srBtn, 'Enable', 'on'); % single-read mode
                set(temp_handles.activeBtn, 'Enable', 'on');
                set(temp_handles.setRateBtn, 'Enable','on');
                set(temp_handles.offsetBtn, 'Enable','on');
                set(temp_handles.xOffset, 'Enable', 'on');
                set(temp_handles.yOffset, 'Enable', 'on');
                set(temp_handles.zOffset, 'Enable', 'on');
                set(temp_handles.osRatioDrpDwn, 'Enable', 'on');
                set(temp_handles.adcRateDrpDwn, 'Enable', 'on');
                set(temp_handles.outRateDrpDwn, 'Enable', 'on'); 
                guidata(hObject,temp_handles);
            case 43
                disp('Single-Read Mode Write Successful');
                set(temp_handles.srBtn,'Value',0);
                set(temp_handles.standbyBtn,'Value',1);
                guidata(hObject,temp_handles);
            case 44
                disp('Active Mode Write Successful'); 
%                 set(temp_handles.commandList, 'Enable', 'off');
%                 %set(temp_handles.standbyBtn, 'Enable', 'off');
%                 set(temp_handles.srBtn, 'Enable', 'off'); % single-read mode
%                 set(temp_handles.activeBtn, 'Enable', 'off');
%                 set(temp_handles.setRateBtn, 'Enable','off');
%                 set(temp_handles.offsetBtn, 'Enable','off');
%                 set(temp_handles.xOffset, 'Enable', 'off');
%                 set(temp_handles.yOffset, 'Enable', 'off');
%                 set(temp_handles.zOffset, 'Enable', 'off');
%                 set(temp_handles.osRatioDrpDwn, 'Enable', 'off');
%                 set(temp_handles.adcRateDrpDwn, 'Enable', 'off');
%                 set(temp_handles.outRateDrpDwn, 'Enable', 'off');                 
%                 guidata(hObject,temp_handles);
            case 45
                disp('Triggered Mode Write Successful');
            case 46
                
                data = get(temp_handles.uitable,'Data');                
                data(xposFR,1) = 1;                
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);
                warndlg('Enable Fast-Read Successful');
            case 47
                
                data = get(temp_handles.uitable,'Data');                
                data(xposFR,1) = 0;                
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);
                warndlg('Disable Fast-Read Successful');
            case 48
                
                data = get(temp_handles.uitable,'Data');                
                data(xposFR,1) = 0;       
                data(xposOFF,1) = 0;   
                set(temp_handles.uitable,'Data',data);  
                guidata(hObject,temp_handles);
                warndlg('Default (Ctrl2Reg) Write Successful'); 
            case 49
                warndlg('Enable Auto-Reset Successful');
            case 50
                warndlg('Disable Auto-Reset Successful');
            case 51
                
                data = get(temp_handles.uitable,'Data');                
                data(xposOFF,1) = 1;                
                set(temp_handles.uitable,'Data',data); 
                guidata(hObject,temp_handles);
                warndlg('Enable User-Offset Successful');
            case 52

                data = get(temp_handles.uitable,'Data');                
                data(xposOFF,1) = 0;                
                set(temp_handles.uitable,'Data',data);     
                guidata(hObject,temp_handles);
                warndlg('Disable User-Offset Successful');
            case 53
                warndlg('Magnetometer Refresh Procedure Activated');                
            case 54
                
                set(temp_handles.commandList, 'Enable', 'on');
                
                set(temp_handles.standbyBtn, 'Enable', 'on');
                set(temp_handles.standbyBtn, 'Value', 1);
                set(temp_handles.srBtn, 'Enable', 'on'); % single-read mode
                set(temp_handles.activeBtn, 'Enable', 'on');
                set(temp_handles.setRateBtn, 'Enable','on');
                set(temp_handles.offsetBtn, 'Enable','on');
                set(temp_handles.xOffset, 'Enable', 'on');
                set(temp_handles.yOffset, 'Enable', 'on');
                set(temp_handles.zOffset, 'Enable', 'on');
                set(temp_handles.osRatioDrpDwn, 'Enable', 'on');
                set(temp_handles.adcRateDrpDwn, 'Enable', 'on');
                set(temp_handles.outRateDrpDwn, 'Enable', 'on'); 
                data = zeros(11,3);
                set(temp_handles.uitable,'Data',data);
                guidata(hObject,temp_handles);
                warndlg('Reset All Settings (MAG)');
            case 55
                disp('PSOC: MAG Data Ready');
                fwrite(XBee, 34, 'uint8');       
            case 56
                warndlg('PSOC: ERROR MAG Busy');
            case 57
                warndlg('PSOC: ERROR Data Array NOT Received');
            case 58
                warndlg('PSOC: ERROR MAG Read Failed');
            case 59
                warndlg('PSOC: ERROR MAG Write Failed');
            case 60
                warndlg('PSOC: ERROR Command Value Out Of Range');   
            case 61 %added for offset write
                display('Offset Write Successful');
                
            otherwise
                if ((Command_Received >= 2) || (Command_Received <= 33))
                    data = get(temp_handles.uitable,'Data');
                    tempbuffer = temp_handles.sampRateBuffer; % order of [adc odr osr]
                    data(xposADC,1) = tempbuffer(1);
                    data(xposODR,1) = tempbuffer(2);
                    data(xposOSR,1) = tempbuffer(3);
                    set(temp_handles.uitable,'Data',data);
                    guidata(hObject,temp_handles);
                    warndlg('Sampling and Data Rate Setting Successful');
                else
                    disp('Command Value Undefined');
                end
               
        end
        guidata(hObject,temp_handles);
    end 
end



function serialPortNameBox_Callback(hObject, eventdata, handles)
% hObject    handle to serialPortNameBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of serialPortNameBox as text
%        str2double(get(hObject,'String')) returns contents of serialPortNameBox as a double
    %set(handles.portName, 'String', 'COM5');
    handles.portName = get(hObject, 'String');
    
end

% --- Executes during object creation, after setting all properties.
function serialPortNameBox_CreateFcn(hObject, eventdata, handles)
% hObject    handle to serialPortNameBox (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

end


% --- Executes on button press in portConnectBtn.
function portConnectBtn_Callback(hObject, eventdata, handles)
% hObject    handle to portConnectBtn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    
    %check if port is not connected
    if (get(handles.portConnectStatus, 'Value') == 0)
        fclose(instrfind);
        fopen(handles.XBee);
        disp(strcat('Serial Port is ', handles.XBee.Status));
        %add code to initiate 'Who am I' function
        % ----- code inserted here ------
        % ----- code inserted here ------
        
        %if opened successfully, enable command buttons
        if strcmp(handles.XBee.Status, 'open')
            set(hObject, 'String', 'Close Port');
            set(handles.portConnectStatus, 'Value', 1);
            set(handles.commandList, 'Enable', 'on');
            set(handles.standbyBtn, 'Enable', 'on');
            set(handles.srBtn, 'Enable', 'on');
            set(handles.activeBtn, 'Enable', 'on');
            set(handles.sendButton, 'Enable', 'on');
            set(handles.setRateBtn, 'Enable','on');
            % Keith Edit
            %set(handles.figure1, 'Interruptible', 'off'); %make figure callbacks NON-interruptible
            set(handles.standbyBtn, 'Value',1); %make sure initial mode is standby
            set(handles.offsetBtn, 'Enable','on');
            set(handles.xOffset, 'Enable', 'on');
            set(handles.yOffset, 'Enable', 'on');
            set(handles.zOffset, 'Enable', 'on');
            set(handles.osRatioDrpDwn, 'Enable', 'on');
            set(handles.adcRateDrpDwn, 'Enable', 'on');
            set(handles.outRateDrpDwn, 'Enable', 'on');
            data = zeros(11,3);
            set(handles.uitable,'Data',data);
        elseif strcmp(handles.XBee.Status, 'closed')
            disp('unable to open port');
            fclose(instrfind);
        end
    %else if port is already connected, close it    
    else
        %fwrite(handles.XBee, 42, 'uint8'); %write MAG to standby before close
        %set(handles.standbyBtn, 'Value',1); %make sure at closing that mode is standby
        %pause(1); %delay for 1 sec, to let write finish
      
        fclose(handles.XBee);
        set(hObject, 'String', 'Open Port');
        set(handles.portConnectStatus, 'Value', 0);
        set(handles.commandList, 'Enable', 'off');
        set(handles.standbyBtn, 'Enable', 'off');
        set(handles.srBtn, 'Enable', 'off');
        set(handles.activeBtn, 'Enable', 'off');
        set(handles.setRateBtn, 'Enable','off');
        % Keith Edit
        set(handles.offsetBtn, 'Enable','off');
        set(handles.xOffset, 'Enable', 'off');
        set(handles.yOffset, 'Enable', 'off');
        set(handles.zOffset, 'Enable', 'off');
        set(handles.osRatioDrpDwn, 'Enable', 'off');
        set(handles.adcRateDrpDwn, 'Enable', 'off');
        set(handles.outRateDrpDwn, 'Enable', 'off');
        disp(strcat('Serial Port is ', handles.XBee.Status));
        
    end
        
end

% --- Executes on button press in portConnectStatus.
function portConnectStatus_Callback(hObject, eventdata, handles)
% hObject    handle to portConnectStatus (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of portConnectStatus
end

% --------------------------------------------------------------------
function serialPortPanel_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to serialPortPanel (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp('hey');
end


% --- Executes on key press with focus on figure1 and none of its controls.
function figure1_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  structure with the following fields (see FIGURE)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)
disp('Key pressed');
end

% --- Executes when figure1 is resized.
function figure1_ResizeFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp('figure1_ResizeFcn');
end


% --- Executes during object creation, after setting all properties.
function figure1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called
disp('figure1_CreateFcn');
end


% --- Executes on mouse press over axes background.
function signalAxes_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to signalAxes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end

% --- Executes during object creation, after setting all properties.
function signalAxes_CreateFcn(hObject, eventdata, handles)
% hObject    handle to signalAxes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate signalAxes
end


% --- Executes on mouse press over figure background, over a disabled or
% --- inactive control, or over an axes background.
function figure1_WindowButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to figure1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over portConnectBtn.
function portConnectBtn_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to portConnectBtn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
end


% --- Executes on button press in setRateBtn.
function setRateBtn_Callback(hObject, eventdata, handles)
% hObject    handle to setRateBtn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    %sendRate(hObject,eventdata,handles);
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
%     numADC = str2num(adcRate);
%     numODR = str2num(outRate);
% 	numOSR = str2num(osRatio);
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


% --- Executes on button press in offsetBtn.
function offsetBtn_Callback(hObject, eventdata, handles)
% hObject    handle to offsetBtn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    XBee = handles.XBee;
    
%     xOffset = str2double(get(handles.xOffset, 'String'));
%     yOffset = str2double(get(handles.yOffset, 'String'));
%     zOffset = str2double(get(handles.zOffset, 'String'));
%     xOffset = cast(xOffset,'int16');
%     yOffset_2 = cast(xOffset,'int16');
%     zOffset_2 = cast(xOffset,'int16');
    xOffset = cellstr(get(handles.xOffset, 'String'));
    xOffset = xOffset{get(handles.xOffset, 'Value')};
    xOffset = str2double(xOffset)
    yOffset = cellstr(get(handles.yOffset, 'String'));
    yOffset = yOffset{get(handles.yOffset, 'Value')};
    yOffset = str2double(yOffset)
    zOffset = cellstr(get(handles.zOffset, 'String'));
    zOffset = zOffset{get(handles.zOffset, 'Value')};
    zOffset = str2double(zOffset)
%     max_val = cast(1000,'int16'); %minimum value to enter
%     min_val = cast(-1000,'int16'); %minimum value to enter
    max_val = 1000;
    min_val = -1000;
    max_check = ([xOffset yOffset zOffset]>max_val);
    min_check = ([xOffset yOffset zOffset]<min_val);
    if (any(max_check)|any(min_check))
        warndlg('Values Invalid. Acceptable Range is -1000 to 1000');
    else
        xOffset = xOffset*10; % For conversion from uT to bit counts
        x_8 = typecast(int16(xOffset),'uint8')% produces x_8(2) = MSB, x_8(1)=LSB
        yOffset = yOffset*10;
        y_8 = typecast(int16(yOffset),'uint8')
        zOffset = zOffset*10;
        z_8 = typecast(int16(zOffset),'uint8')
        handles.offsetValues = [x_8(2) x_8(1) y_8(2) y_8(1) z_8(2) z_8(1)];
        handles.offsetFlag = 1; %set flag for offset procedure
        fwrite(XBee, 1, 'uint8'); %Command Value for write offset
    end
%     fwrite(XBee, 1, 'uint8'); %Command Value for write offset
%     fwrite(XBee, xOffset, 'int16');%check that it sends in right order
%     fwrite(XBee, yOffset_2, 'int16');
%     fwrite(XBee, zOffset_2, 'int16');
end



% --- Executes during object creation, after setting all properties.
function xOffset_CreateFcn(hObject, eventdata, handles)
% hObject    handle to xOffset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

end


% --- Executes during object creation, after setting all properties.
function yOffset_CreateFcn(hObject, eventdata, handles)
% hObject    handle to yOffset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
end


% --- Executes during object creation, after setting all properties.
function zOffset_CreateFcn(hObject, eventdata, handles)
% hObject    handle to zOffset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
end



% --- Executes during object creation, after setting all properties.
function osRatioDrpDwn_CreateFcn(hObject, eventdata, handles)
% hObject    handle to osRatioDrpDwn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
end



% --- Executes during object creation, after setting all properties.
function outRateDrpDwn_CreateFcn(hObject, eventdata, handles)
% hObject    handle to outRateDrpDwn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
end

function adcRateDrpDwn_CreateFcn(hObject, eventdata, handles)
% hObject    handle to osRatioDrpDwn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns osRatioDrpDwn contents as cell array
%        contents{get(hObject,'Value')} returns selected item from osRatioDrpDwn
end
