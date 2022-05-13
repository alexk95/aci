@ECHO OFF
REM This script requires the following environment variables to be set:
REM 1. ACI_ROOT
REM 2. DEVENV_ROOT
REM 3. UI_CORE_ROOT

IF "%ACI_ROOT%" == "" (
	ECHO Please specify the following environment variables: ACI_ROOT
	goto END
)

IF "%DEVENV_ROOT%" == "" (
	ECHO Please specify the following environment variables: DEVENV_ROOT
	goto END
)

IF "%UI_CORE_ROOT%" == "" (
	ECHO Please specify the following environment variables: UI_CORE_ROOT
	goto END
)

IF "%QTDIR_ROOT%" == "" (
	ECHO Please specify the following environment variables: QTDIR_ROOT
	goto END
)

REM Setup UI eviroment
CALL "%ACI_ROOT%\setupEnvironment.bat"

ECHO Launching development enviroment
START "" "%DEVENV_ROOT%\devenv.exe" "%ACI_DEFAULT_ODBC%\odbc.vcxproj"

:END

