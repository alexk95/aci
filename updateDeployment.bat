@ECHO OFF

REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR_ROOT
REM 4. CNC_ROOT

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

IF "%CNC_ROOT%" == "" (
	ECHO Please specify the following environment variables: CNC_ROOT
	goto END
)

REM Setup UI eviroment
CALL "%CNC_ROOT%\MasterBuild\setupEnvironment.bat"

IF "%ACI_DEPLOYMENT_DIR%" == "" (
    SET ACI_DEPLOYMENT_DIR="%CNC_ROOT%\Additional_tools\aci\Deployment"
)

DEL "%ACI_DEPLOYMENT_DIR%\aci.exe"
COPY "%CNC_ROOT%\Additional_tools\aci\x64\Release\aci.exe" "%ACI_DEPLOYMENT_DIR%"