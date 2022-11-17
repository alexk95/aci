@ECHO OFF

REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR_ROOT
REM 4. ACI_ROOT

IF "%UI_CORE_ROOT%" == "" (
	ECHO Please specify the following environment variables: UI_CORE_ROOT
	goto END
)

IF "%DEVENV_ROOT%" == "" (
	ECHO Please specify the following environment variables: DEVENV_ROOT
	goto END
)

IF "%QTDIR_ROOT%" == "" (
	ECHO Please specify the following environment variables: QTDIR_ROOT
	goto END
)

IF "%ACI_ROOT%" == "" (
	ECHO Please specify the following environment variables: ACI_ROOT
	goto END
)

REM Setup UI eviroment
CALL "%ACI_ROOT%\setupEnvironment.bat"

REM Delete old scripts
DEL "%ACI_ROOT%\Scripts\merge.dll"
DEL "%ACI_ROOT%\Scripts\odbc.dll"
DEL "%ACI_ROOT%\Scripts\default.dll"
DEL "%ACI_ROOT%\Scripts\source.dll"
DEL "%ACI_ROOT%\Scripts\cleantext.dll"

DEL "%ACI_ROOT%\ScriptsD\merge.dll"
DEL "%ACI_ROOT%\ScriptsD\odbc.dll"
DEL "%ACI_ROOT%\ScriptsD\default.dll"
DEL "%ACI_ROOT%\ScriptsD\source.dll"
DEL "%ACI_ROOT%\ScriptsD\cleantext.dll"

REM Copy new scripts
COPY "%ACI_DEFAULT_MERGE%\x64\Release\merge.dll" "%ACI_ROOT%\Scripts"
COPY "%ACI_DEFAULT_ODBC%\x64\Release\odbc.dll" "%ACI_ROOT%\Scripts"
COPY "%ACI_DEFAULT_DEFAULT%\x64\Release\default.dll" "%ACI_ROOT%\Scripts"
COPY "%ACI_DEFAULT_SOURCE%\x64\Release\source.dll" "%ACI_ROOT%\Scripts"
COPY "%ACI_DEFAULT_CLEANTEXT%\x64\Release\cleantext.dll" "%ACI_ROOT%\Scripts"

COPY "%ACI_DEFAULT_MERGE%\x64\Debug\merge.dll" "%ACI_ROOT%\ScriptsD"
COPY "%ACI_DEFAULT_ODBC%\x64\Debug\odbc.dll" "%ACI_ROOT%\ScriptsD"
COPY "%ACI_DEFAULT_DEFAULT%\x64\Debug\default.dll" "%ACI_ROOT%\ScriptsD"
COPY "%ACI_DEFAULT_SOURCE%\x64\Debug\source.dll" "%ACI_ROOT%\ScriptsD"
COPY "%ACI_DEFAULT_CLEANTEXT%\x64\Debug\cleantext.dll" "%ACI_ROOT%\ScriptsD"
