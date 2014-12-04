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

% Last Modified by GUIDE v2.5 03-Dec-2014 18:50:18

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
%ADDED KEITH
%XYZ_Array = [0 0 0;0 0 0; 0 0 0];
%handles.XYZ = XYZ_Array;

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
    while(XBee.BytesAvailable > 0)
        Command_Received = fread(XBee,1, 'uint8');
        
        switch Command_Received
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
                %data = guidata(hObject);
                %temp2 = data(2,:);
                %temp3 = data(3,:);
                
                %data = [temp2;temp3; x y z];
                
                %guidata(hObject,data);
                
                temp_handles = guidata(hObject);
                c = temp_handles.XYZ;
                %handles.temp2 = c(2,:);
                %handles.temp3 = c(3,:);
                c(1,:) = temp_handles.XYZ(2,:);
                c(2,:) = temp_handles.XYZ(3,:);
                c(3,:) = [x y z];
                temp_handles.XYZ = c;
                %handles.XYZ(3,:) = [x y z];
                %[x y z] = c(1,:);
                %handles.XYZ = [handles.temp2;handles.temp3;x y z]
                %handles.XYZ = [1 2 3; 4 5 6; 7 8 9];
                %bar3(handles.signalAxes,handles.XYZ);
                %set(handles.signalAxes, 'YTickLabel', ['old '; 'last'; 'new ']);
                %set(handles.signalAxes, 'XTickLabel', ['X'; 'Y'; 'Z']);   
                bar3(temp_handles.signalAxes,temp_handles.XYZ,'grouped');
                %set(temp_handles.signalAxes,'XTickLabel',['Oldest';'Last';'New']);
                set(temp_handles.signalAxes,'YTickLabel',['O';'L';'N']);
                guidata(hObject,temp_handles);
                
            case 35
                val = fread(XBee, 1, 'uint8');
                disp('Control Register 1:');
                disp(val);
            case 36
                val = fread(XBee, 1, 'uint8');
                disp('Control Register 2:');
                disp(val);
            case 37
                val = fread(XBee, 1, 'uint8');
                disp('Data Ready Status Register:');
                disp(val);
            case 38
                val = fread(XBee, 1, 'uint8');
                disp('System Mode Register:');
                disp(val);                
            case 39
                val = fread(XBee, 1, 'int8');
                disp('Die Temperature Register:');
                disp(val);                
            case 40
                val = fread(XBee, 1, 'uint8');
                disp('Who Am I Register:');
                disp(val);
            case 41
                disp('Default (Ctrl1Reg) Write Successful');
            case 42
                disp('Standby Mode Write Successful');
            case 43
                disp('Single-Read Mode Write Successful');
            case 44
                disp('Active Mode Write Successful'); 
            case 45
                disp('Triggered Mode Write Successful');
            case 46
                disp('Enable Fast-Read Successful');
            case 47
                disp('Disable Fast-Read Successful');
            case 48
                disp('Default (Ctrl2Reg) Write Successful'); 
            case 49
                disp('Enable Auto-Reset Successful');
            case 50
                disp('Disable Auto-Reset Successful');
            case 51
                disp('Enable User-Offset Successful');
            case 52
                disp('Disable User-Offset Successful');
            case 53
                disp('Magnetometer Refresh Procedure Activated');                
            case 54
                disp('Reset All Settings (MAG)');
            case 55
                disp('PSOC: MAG Data Ready');
                fwrite(XBee, 34, 'uint8')          
            case 56
                disp('PSOC: ERROR MAG Busy');
            case 57
                disp('PSOC: ERROR Data Array NOT Received');
            case 58
                disp('PSOC: ERROR MAG Read Failed');
            case 59
                disp('PSOC: ERROR MAG Write Failed');
            case 60
                disp('PSOC: ERROR Command Value Out Of Range');   
            otherwise
                %if (Command_Received >= 41) && (Command_Received <= 53)
                %    disp('Write Successful');
                %else
                %    disp('Value Undefined');
                %end
                disp('Command Value Undefined');
        end         
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
        elseif strcmp(handles.XBee.Status, 'closed')
            disp('unable to open port');
            fclose(instrfind);
        end
    %else if port is already connected, close it    
    else
        fclose(handles.XBee);
        set(hObject, 'String', 'Open Port');
        set(handles.portConnectStatus, 'Value', 0);
        set(handles.commandList, 'Enable', 'off');
        set(handles.standbyBtn, 'Enable', 'off');
        set(handles.srBtn, 'Enable', 'off');
        set(handles.activeBtn, 'Enable', 'off');
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
disp('figure1_ResizeFcn');
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

% --- Executes on selection change in popupmenu2.
function popupmenu2_Callback(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns popupmenu2 contents as cell array
%        contents{get(hObject,'Value')} returns selected item from popupmenu2
end

% --- Executes during object creation, after setting all properties.
function popupmenu2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popupmenu2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
    if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
        set(hObject,'BackgroundColor','white');
    end
end
