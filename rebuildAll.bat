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

REM Setup enviroment
CALL "%ACI_ROOT%\setupEnvironment.bat"

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

SET ACI_EMPTY=:

ECHO =============================================================================
ECHO Build the libraries 
ECHO =============================================================================
ECHO %ACI_EMPTY%

ECHO ===============================================================
ECHO Build Library: ACI CORE
ECHO ===============================================================
CALL "%ACI_CORE%\build.bat" %1 %2

ECHO =============================================================================
ECHO Build the executables
ECHO =============================================================================
ECHO %ACI_EMPTY%

ECHO ===============================================================
ECHO Build Executable: ACI
ECHO ===============================================================
CALL "%ACI_UI%\build.bat" %1 %2

ECHO =============================================================================
ECHO Build scripts
ECHO =============================================================================
ECHO %ACI_EMPTY%

ECHO ===============================================================
ECHO Build Library: Script: merge
ECHO ===============================================================
CALL "%ACI_DEFAULT_MERGE%\build.bat" %1 %2

ECHO ===============================================================
ECHO Build Library: Script: odbc
ECHO ===============================================================
CALL "%ACI_DEFAULT_ODBC%\build.bat" %1 %2

ECHO ===============================================================
ECHO Build Library: Script: default
ECHO ===============================================================
CALL "%ACI_DEFAULT_DEFAULT%\build.bat" %1 %2

ECHO ===============================================================
ECHO Build Library: Script: source
ECHO ===============================================================
CALL "%ACI_DEFAULT_SOURCE%\build.bat" %1 %2

ECHO =============================================================================
ECHO Create the buildlog summary
ECHO =============================================================================

if "%2"=="" (
	SET searchString="Rebuild All:"
) ELSE IF "%2"=="REBUILD" (
	SET searchString="Rebuild All:"
) ELSE (
	SET searchString="Build:"
)

FIND %searchString% buildLog_Debug.txt buildLog_Release.txt > buildLog_Summary.txt
