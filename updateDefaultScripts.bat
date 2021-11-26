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
DEL "%ACI_ROOT%\Scripts\default.dll"

REM Copy new scripts
COPY "%ACI_DEFAULT_MERGE%\x64\Release\merge.dll" "%ACI_ROOT%\Scripts"
COPY "%ACI_DEFAULT_DEFAULT%\x64\Release\default.dll" "%ACI_ROOT%\Scripts"
