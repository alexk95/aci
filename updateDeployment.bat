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

IF "%ACI_DEPLOYMENT_DIR%" == "" (
    SET ACI_DEPLOYMENT_DIR="%ACI_ROOT%\Deployment"
)

REM Delete old files
DEL "%ACI_DEPLOYMENT_DIR%\uiCore.dll"
DEL "%ACI_DEPLOYMENT_DIR%\TabToolbar.dll"
DEL "%ACI_DEPLOYMENT_DIR%\aci.exe"
DEL "%ACI_DEPLOYMENT_DIR%\aciCore.dll"
RMDIR /S /Q "%ACI_DEPLOYMENT_DIR%\Scripts"

REM Copy the Application
COPY "%UI_CORE_ROOT%\x64\Release\uiCore.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar\src\TabToolbar\Release\TabToolbar.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%ACI_UI%\x64\Release\aci.exe" "%ACI_DEPLOYMENT_DIR%"
COPY "%ACI_CORE%\x64\Release\aciCore.dll" "%ACI_DEPLOYMENT_DIR%"

REM Copy the scripts
MKDIR "%ACI_DEPLOYMENT_DIR%\Scripts"
XCOPY /S "%ACI_ROOT%\Scripts" "%ACI_DEPLOYMENT_DIR%\Scripts"
