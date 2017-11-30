; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Emily"
#define MyAppVersion "0.9"
#define MyAppPublisher "nstrikos@yahoo.gr"
#define MyAppExeName "emily.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{258935EE-1D44-4A40-8F72-D543EBB19EF5}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName={pf}\{#MyAppName}
DisableDirPage=yes
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
;LicenseFile=C:\Users\nick\Desktop\projects\Emily\build\licenses\gpl-3.0.txt
OutputDir=C:\Users\nick\Desktop\projects\Emily\inno-setup
SetupIconFile=C:\Users\nick\Desktop\projects\Emily\build\resources\sound.ico
OutputBaseFilename=emily
Compression=lzma
SolidCompression=yes
;PrivilegesRequired=admin

;[Languages]
;Name: "greek"; MessagesFile: "compiler:Languages\Greek.isl"
[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "el"; MessagesFile: "compiler:Languages\Greek.isl"

[Files]
Source: "C:\Users\nick\Desktop\projects\Emily\build\emily.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\nick\Desktop\projects\Emily\build\marytts\*"; DestDir: "{app}\marytts\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\translations\*"; DestDir: "{app}\translations\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\platforms\*"; DestDir: "{app}\platforms\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\resources\*"; DestDir: "{app}\resources\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\licenses\*"; DestDir: "{app}\licenses\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\mediaservice\*"; DestDir: "{app}\mediaservice\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\resources\openmary.py"; DestDir: "{userappdata}\nvda\addons\Emily\synthDrivers\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Users\nick\Desktop\projects\Emily\build\resources\manifest.ini"; DestDir: "{userappdata}\nvda\addons\Emily\"; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{userstartup}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

;[Registry]
;Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: {#MyAppName}; ValueData: """{app}\{#MyAppExeName}"""; Flags: uninsdeletevalue


