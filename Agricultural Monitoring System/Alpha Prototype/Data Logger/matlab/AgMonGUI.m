function varargout = AgMonGUI(varargin)
% AGMONGUI MATLAB code for AgMonGUI.fig
%      AGMONGUI, by itself, creates a new AGMONGUI or raises the existing
%      singleton*.
%
%      H = AGMONGUI returns the handle to a new AGMONGUI or the handle to
%      the existing singleton*.
%
%      AGMONGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in AGMONGUI.M with the given input arguments.
%
%      AGMONGUI('Property','Value',...) creates a new AGMONGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before AgMonGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to AgMonGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help AgMonGUI

% Last Modified by GUIDE v2.5 21-Jan-2015 12:45:49

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @AgMonGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @AgMonGUI_OutputFcn, ...
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


% --- Executes just before AgMonGUI is made visible.
function AgMonGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to AgMonGUI (see VARARGIN)

% Choose default command line output for AgMonGUI
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes AgMonGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);

x = [0 4];
y1 = [1 1];
y2 = [2 2];
y3 = [3 3];
h = plot(gca, x, y1, x, y2, x, y3);
get(h, 'XLabel');


% --- Outputs from this function are returned to the command line.
function varargout = AgMonGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes during object creation, after setting all properties.
function plot_axes_CreateFcn(hObject, eventdata, handles)
% hObject    handle to plot_axes (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate plot_axes
