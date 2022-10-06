@ECHO OFF
REM This script requires the following environment variables to be set:
REM 1. ACI_ROOT
REM 2. DEVENV_ROOT
REM 3. UI_CORE_ROOT

REM Ensure that the setup will only be performed once
if "%ACI_ENV_DEFINED%"=="1" (
	goto END
)

SET ACI_ENV_DEFINED=1

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

REM Set project paths
SET ACI_UI=%ACI_ROOT%\Interpreter
SET ACI_CORE=%ACI_ROOT%\Core
SET ACI_EXAMPLE=%ACI_ROOT%\ScriptExample
SET ACI_DEFAULT_MERGE=%ACI_ROOT%\DefaultScripts\merge
SET ACI_DEFAULT_ODBC=%ACI_ROOT%\DefaultScripts\odbc
SET ACI_DEFAULT_DEFAULT=%ACI_ROOT%\DefaultScripts\default
SET ACI_DEFAULT_SOURCE=%ACI_ROOT%\DefaultScripts\source

REM Setup UI Core env
CALL "%UI_CORE_ROOT%\setupEnvironment.bat"

:END