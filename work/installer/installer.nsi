;-------------------------------------------------------------------------------
;
;   Nullsoft Installer script for "Nebula3 SDK"
;
;   (C) 2007 RadonLabs GmbH
;-------------------------------------------------------------------------------

;-------------------------------------------------------------------------------
;   Include Modern UI

    !include "MUI.nsh"

;-------------------------------------------------------------------------------
;   Defines
    
    !define COMPANY "Radon Labs GmbH"
    !define PRODUCT "Nebula3 SDK (Mar 2010)"
    !define SETUP   "N3SDK_Mar2010"
    !define SRCDIR "..\.."
    !define STARTMENU "$SMPROGRAMS\${PRODUCT}"

;-------------------------------------------------------------------------------
;   General

    Name "${PRODUCT}"
    OutFile "${SRCDIR}\${SETUP}.exe"
    XPStyle on
    CrcCheck off
    RequestExecutionLevel user    

    ; default installation directory
    InstallDir "$PROGRAMFILES\${PRODUCT}"

    ; Get installation folder from registry if available
    InstallDirRegKey HKCU "Software\${COMPANY}\${PRODUCT}" "installdir"
       
;-------------------------------------------------------------------------------
;   Interface Settings

    !define MUI_ABORTWARNING
    !define MUI_ICON "install.ico"
    !define MUI_UNICON "uninstall.ico"
    !define MUI_HEADERIMAGE
    !define MUI_HEADERIMAGE_BITMAP "header.bmp"
    !define MUI_HEADERIMAGE_UNBITMAP "header.bmp"
    !define MUI_WELCOMEFINISHPAGE_BITMAP "wizard.bmp"
    !define MUI_UNWELCOMEFINISHPAGE_BITMAP "wizard.bmp"
    !define MUI_INSTFILESPAGE_PROGRESSBAR "smooth"

;--------------------------------
;   Pages

    !insertmacro MUI_PAGE_WELCOME
    !insertmacro MUI_PAGE_DIRECTORY
    !insertmacro MUI_PAGE_INSTFILES
    !insertmacro MUI_PAGE_FINISH
  
    !insertmacro MUI_UNPAGE_WELCOME
    !insertmacro MUI_UNPAGE_CONFIRM
    !insertmacro MUI_UNPAGE_INSTFILES
    !insertmacro MUI_UNPAGE_FINISH
    
;--------------------------------
;   Languages
 
    !insertmacro MUI_LANGUAGE "English"
    
;-------------------------------------------------------------------------------
;   Installer Sections     

Section "-Everything" Section_EverythingFiles

    ; root
    SetOutPath "$INSTDIR"
    File /x "*xbox360*" /x "*wii*" /x "*ps3*" "${SRCDIR}\*.msbuild"
    File /x "*xbox360*" /x "*wii*" /x "*ps3*" "${SRCDIR}\*.xml"    

    ; bin
    SetOutPath "$INSTDIR\bin"
    File /x "*xbox360*" /x "*wii*" /x "*ps3*" "${SRCDIR}\bin\*.tcl"
    SetOutPath "$INSTDIR\bin\win32"
    File /x "*xbox360*" /x "*wii*" /x "*ps3*" /x *.debug.* "${SRCDIR}\bin\win32\*.exe"
    
    ; code
    SetOutPath "$INSTDIR\code"
    File "${SRCDIR}\code\*_win32.epk"
    File /x "*wii.tcl" "${SRCDIR}\code\*.tcl"
    File "${SRCDIR}\code\*.rules"    
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" /x "bink*" "${SRCDIR}\code\*.cc" 
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" /x "bink*" "${SRCDIR}\code\*.c" 
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" /x "bink*" "${SRCDIR}\code\*.h" 
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" /x "bink*" "${SRCDIR}\code\*.hpp"     
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" /x "bink*" "${SRCDIR}\code\*.dox" 
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" /x "bink*" "${SRCDIR}\code\*.nidl"

    ; special cases
    SetOutPath "$INSTDIR\code\toolkit\toolkitutil"
    File "${SRCDIR}\code\toolkit\toolkitutil\*.cc"
    File "${SRCDIR}\code\toolkit\toolkitutil\*.h"

    ; VStudio files
    SetOutPath "$INSTDIR\code\vs9.0"
    File /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" "${SRCDIR}\code\vs9.0\*.sln"
    File /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" "${SRCDIR}\code\vs9.0\*.vcproj"    

    ; lib
    SetOutPath "$INSTDIR\lib"
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x "*scaleform*" "${SRCDIR}\lib\*.lib"

    ; special RakNet stuff
    SetOutPath "$INSTDIR\code\extlibs\raknet"
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" "${SRCDIR}\code\extlibs\raknet\*.lib"
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" "${SRCDIR}\code\extlibs\raknet\*.pdb"

    ; resource stuff
    SetOutPath "$INSTDIR\code\res"
    File /x ".svn" "${SRCDIR}\code\res\*.*"

    ; doc
    SetOutPath "$INSTDIR\doc"
    File "${SRCDIR}\doc\*.chm"
    File "${SRCDIR}\doc\*.txt"
    SetOutPath "$INSTDIR\doc\doxygen"
    File /x ".svn" /x ".#*" "${SRCDIR}\doc\doxygen\*.*"
    SetOutPath "$INSTDIR\doc\nebula3"
    File /x ".svn" /x ".#*" "${SRCDIR}\doc\nebula3\*.*"

    ; schemas
    SetOutPath "$INSTDIR\schemas"
    File "${SRCDIR}\schemas\*.xsd"
    
    ; work
    SetOutPath "$INSTDIR\work"
    File /r /x ".svn" /x "*xbox360*" /x "*wii*" /x "*ps3*" /x ".#*" /x "textures" /x "gfxlib" /x "levels" /x "testdata" "${SRCDIR}\work\*.*"

    ; export
    SetOutPath "$INSTDIR"
    File "${SRCDIR}\export_win32.zip"
    SetOutPath "$INSTDIR\export_win32\db"
    File "${SRCDIR}\export_win32\db\*.db4"
;   write registry keys
    WriteRegStr HKCU "Software\${COMPANY}\${PRODUCT}" "installdir" "$INSTDIR"

;   write uninstall information to the registry   
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" "$INSTDIR\Uninstall.exe"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayIcon" "$INSTDIR\bin\win32\nebula.ico,0"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "-StartMenuEntries" Section_StartMenuEntries

    ; Delete Old Start Menu Shortcuts
    IfFileExists "$SMPROGRAMS\${PRODUCT}" "" +2
        RmDir /r "$SMPROGRAMS\${PRODUCT}"

    ; this sets the "current working directory" for the shortcuts
    SetOutPath "$INSTDIR\bin\win32"
    CreateDirectory "${STARTMENU}"
    ; CreateDirectory "${STARTMENU}\Lighting Test"
    ; CreateDirectory "${STARTMENU}\App Layer Test"

    ; CreateShortCut "${STARTMENU}\Lighting Test\No MSAA.lnk" "$INSTDIR\bin\win32\testviewer.exe" "-w 640 -h 480"
    ; CreateShortCut "${STARTMENU}\Lighting Test\2x MSAA.lnk" "$INSTDIR\bin\win32\testviewer.exe" "-w 640 -h 480 -aa Low"
    ; CreateShortCut "${STARTMENU}\Lighting Test\4x MSAA.lnk" "$INSTDIR\bin\win32\testviewer.exe" "-w 640 -h 480 -aa Medium"
    ; CreateShortCut "${STARTMENU}\App Layer Test\No MSAA.lnk" "$INSTDIR\bin\win32\testgame.exe" "-w 640 -h 480"
    ; CreateShortCut "${STARTMENU}\App Layer Test\2x MSAA.lnk" "$INSTDIR\bin\win32\testgame.exe" "-w 640 -h 480 -aa Low"
    ; CreateShortCut "${STARTMENU}\App Layer Test\4x MSAA.lnk" "$INSTDIR\bin\win32\testgame.exe" "-w 640 -h 480 -aa Medium"
    ; CreateShortCut "${STARTMENU}\Connect Web Browser.lnk" "http://127.0.0.1:2100"
    CreateShortCut "${STARTMENU}\Browse Files.lnk" "$WINDIR\explorer.exe" "$INSTDIR"
    CreateShortCut "${STARTMENU}\Documentation.lnk" "$INSTDIR\doc\nebula3.chm"
    CreateShortCut "${STARTMENU}\License.lnk" "$INSTDIR\doc\license.txt"
    CreateShortCut "${STARTMENU}\Uninstall.lnk" "$INSTDIR\Uninstall.exe" "" "$INSTDIR\Uninstall.exe" 0
SectionEnd
    
;-------------------------------------------------------------------------------
;   Uninstaller Section  
Section "Uninstall"

;   Remove the installation directory
    RMDir /r "$INSTDIR"

;   Delete Start Menu Shortcuts
    Delete "${STARTMENU}\*.*"
    RmDir  "${STARTMENU}"
  
;   Delete Uninstaller And Unistall Registry Entries
    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"  
  
SectionEnd

