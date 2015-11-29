#define AppName "KotOR Savegame Editor"
#define KPFAppName "KPF"
#define Version "3.3.7"
#define Publisher "KSE Team"
#define PublisherURL "http://deadlystream.com/forum/files/file/503-kotor-save-game-editor/"
#define SupportURL "http://mantis.kalebklein.com"
#define ExeName "kse.exe"
#define SetupBaseName "kse_setup"
#define Build "Pre-Release"
#define VCLStylesSkinPath "{localappdata}\VCLStylesSkin"

[Setup]
AppId={{7338BB87-FF18-40E7-957E-905F22021363}
AppName={#AppName}
AppVersion={#Version}
AppVerName={#AppName} {#Version}
AppPublisher={#Publisher}
AppPublisherURL={#PublisherURL}
AppSupportURL={#SupportURL}
DefaultDirName=C:\{#Publisher}\{#AppName}
DefaultGroupName={#Publisher}
LicenseFile=..\..\LICENSE
OutputBaseFilename={#SetupBaseName}-{#Version}-{#Build}
OutputDir=.\
SetupIconFile=boba.ico
Compression=lzma
SolidCompression=yes
WizardImageFile="C:\Program Files (x86)\The Road To Delphi\VCL Styles Inno\Images\WizModernImage-IS.bmp"
WizardSmallImageFile=boba.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[CustomMessages]
DeadlyStream=KSE on DeadlyStream
Support=KSE Support Page
DesktopIcon=Create a desktop icon for %1

[Tasks]
Name: "desktopicon"; Description: "{cm:DesktopIcon,KSE}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "kpficon"; Description: "{cm:DesktopIcon,{#KPFAppName}}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Code]
// Import the LoadVCLStyle function from VclStylesInno.DLL
procedure LoadVCLStyle(VClStyleFile: String); external 'LoadVCLStyleW@files:VclStylesInno.dll stdcall setuponly';
procedure LoadVCLStyle_UnInstall(VClStyleFile: String); external 'LoadVCLStyleW@{#VCLStylesSkinPath}\VclStylesInno.dll stdcall uninstallonly';
// Import the UnLoadVCLStyles function from VclStylesInno.DLL
procedure UnLoadVCLStyles; external 'UnLoadVCLStyles@files:VclStylesInno.dll stdcall setuponly';
procedure UnLoadVCLStyles_UnInstall; external 'UnLoadVCLStyles@{#VCLStylesSkinPath}\VclStylesInno.dll stdcall uninstallonly';

function InitializeSetup(): Boolean;
begin
  ExtractTemporaryFile('Glow.vsf');
  LoadVCLStyle(ExpandConstant('{tmp}\Glow.vsf'));

  result := true
end;

procedure DeinitializeSetup();
begin
  UnLoadVCLStyles;
end;

function InitializeUninstall(): Boolean;
begin
  Result := True;
  LoadVCLStyle_UnInstall(ExpandConstant('{#VCLStylesSkinPath}\Glow.vsf'));
end;

procedure DeinitializeUninstall();
begin
  UnLoadVCLStyles_UnInstall;
end;

[Files]
; Exe files
Source: "kse.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "kpf-qt.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "updater.exe"; DestDir: "{app}"; Flags: ignoreversion

; Images
Source: "boba.bmp"; DestDir: "{app}"; Flags: ignoreversion

; Libraries
Source: "icudt53.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "icuin53.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "icuuc53.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion

; Installer Styles
; Change these locations depending on where you have
; the styles installed on your machine
Source: "C:\Program Files (x86)\The Road To Delphi\VCL Styles Inno\VclStylesinno.dll"; DestDir: {#VCLStylesSkinPath};
Source: "C:\Program Files (x86)\The Road To Delphi\VCL Styles Inno\Styles\Glow.vsf"; DestDir: {#VCLStylesSkinPath};

[Icons]
; ================
; Section: Start Menu
; ================
; Runnables
Name: "{group}\{#AppName}"; Filename: "{app}\{#ExeName}"
Name: "{group}\{cm:UninstallProgram,{#AppName}}"; Filename: "{uninstallexe}"

; URL Links
Name: "{group}\{cm:DeadlyStream}"; Filename: "{#PublisherURL}"
Name: "{group}\{cm:Support}"; Filename: "{#SupportURL}"

; =================
; Sesion: Desktop Icons
; =================
Name: "{commondesktop}\{#AppName}"; Filename: "{app}\{#ExeName}"; Tasks: desktopicon; IconFilename: "{app}\boba.bmp"
Name: "{commondesktop}\{#KPFAppName}"; Filename: "{app}\kpf-qt.exe"; Tasks: kpficon

[Run]
Filename: "{app}\{#ExeName}"; Description: "{cm:LaunchProgram,{#StringChange(AppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\*.txt"
Type: files; Name: "{app}\*.log"
Type: files; Name: "{app}\kse.ini"
