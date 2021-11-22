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

REM Set Qt Environment
IF "%SIM_PLAT_ROOT%" == "" (
    IF "%QDIR%" == "" (
        ECHO Please specify the following environment variable: QDIR
        goto END
    )
)
ELSE (
    SET QDIR=%SIM_PLAT_ROOT%\Third_Party_Libraries\Qt\5.11.2\msvc2017_64
)
SET QTDIR=%QDIR%

REM Set project paths
SET ACI_INTERPRETER=%ACI_ROOT%\Interpreter
SET ACI_API=%ACI_ROOT%\API
SET ACI_EXAMPLE=%ACI_ROOT%\ScriptExample

REM Setup UI Core env
CALL "%UI_CORE_ROOT%\setupEnvironment.bat"

:END