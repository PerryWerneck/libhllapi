!include "MUI2.nsh"
!include "x64.nsh"
!include "FileFunc.nsh"

Name "hllapi"
Caption "hllapi - PW3270 HLLAPI Module"
outfile "hllapi-5.2.19.8-x86_64.exe"

XPStyle on

installDir "$PROGRAMFILES64\hllapi"

# Get installation folder from registry if available
InstallDirRegKey HKLM "Software\hllapi" "InstallLocation"

RequestExecutionLevel admin

# Properties
VIProductVersion "5.2.19.8"
VIFileVersion "19.8.13.14"

VIAddVersionKey "ProductVersion" "5.2.19.8"
VIAddVersionKey "FileVersion" "19.8.13.14"

VIAddVersionKey "ProductName" "hllapi"
VIAddVersionKey "FileDescription" "PW3270 HLLAPI Module"
VIAddVersionKey "LegalCopyright" "GPL-2.0"

# Interface

!define MUI_ABORTWARNING
# !insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

# !insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
# !insertmacro MUI_UNPAGE_FINISH

# Languages
!insertmacro MUI_LANGUAGE "English"

# Section scripts
!include Sections.nsh

# default section
SubSection "hllapi" SecMain

	Section "Core" SecCore

		SetRegView 64
		${DisableX64FSRedirection}

		# define the output path for this file
		setOutPath $SYSDIR
		SetShellVarContext all

		file "/oname=$SYSDIR\hllapi.dll" "..\.bin\Release\hllapi.dll"

		# define uninstaller name
		SetRegView 32

		writeUninstaller $INSTDIR\uninstall.exe

		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi" \
			         "DisplayName" "hllapi - PW3270 HLLAPI Module"

		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi" \
			         "DisplayVersion" "5.2"

		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi" \
			         "UninstallString" "$INSTDIR\uninstall.exe"

		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi" \
			         "InstallLocation" "$INSTDIR"

		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi" \
			         "NoModify" "1"

		WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi" \
			         "NoRepair" "1"

		# Save instalation dir
		WriteRegStr HKCU "Software\hllapi" "" $INSTDIR

	sectionEnd

SubSectionEnd

Section "Uninstall"

	# Always delete uninstaller first
	delete $INSTDIR\uninstaller.exe

	# delete installed files
	delete $SYSDIR\hllapi.dll

	# Remove registry
	SetRegView 32
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\hllapi"
	DeleteRegKey HKLM "Software\hllapi"
	
	SetRegView 64
	DeleteRegKey HKLM "Software\hllapi"

SectionEnd



