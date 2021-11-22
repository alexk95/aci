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

REM Set Qt Environment
IF "%SIM_PLAT_ROOT%" == "" (
    IF "%QDIR%" == "" (
        ECHO Please specify the following environment variable: QDIR
        goto END
    )
)

REM Setup UI eviroment
CALL "%ACI_ROOT%\setupEnvironment.bat"

ECHO Building Project

REM Open project

SET RELEASE=1
SET DEBUG=1

IF "%1"=="RELEASE" (
  SET RELEASE=1
  SET DEBUG=0
)

IF "%1"=="DEBUG" (
  SET RELEASE=0
  SET DEBUG=1
)

SET TYPE=/Rebuild
SET TYPE_NAME=REBUILD

IF "%2"=="BUILD" (
	SET TYPE=/Build
	SET TYPE_NAME=BUILD
)

REM Clean up the build logs
if "%1"=="" (
	DEL buildLog_Debug.txt
	DEL buildLog_Release.txt
)
	
if "%1"=="BOTH" (
	DEL buildLog_Debug.txt
	DEL buildLog_Release.txt
)
	
if "%1"=="RELEASE" (
	DEL buildLog_Release.txt
)
	
if "%1"=="DEBUG" (
	DEL buildLog_Debug.txt
)

IF %DEBUG%==1 (
	ECHO %TYPE_NAME% DEBUG
	"%DEVENV_ROOT%\devenv.exe" "%ACI_EXAMPLE%\MyScript.vcxproj" %TYPE% "Debug|x64" /Out buildLog_Debug.txt
)

IF %RELEASE%==1 (
	ECHO %TYPE_NAME% RELEASE
	"%DEVENV_ROOT%\devenv.exe" "%ACI_EXAMPLE%\MyScript.vcxproj" %TYPE% "Release|x64" /Out buildLog_Release.txt
) 
  
:END

