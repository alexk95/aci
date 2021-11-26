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

IF "%ACI_DEPLOYMENT_DIR%" == "" (
    SET ACI_DEPLOYMENT_DIR="%ACI_ROOT%\Deployment"
)


REM Clean up the Deployment directory
RMDIR /S /Q "%ACI_DEPLOYMENT_DIR%"
MKDIR "%ACI_DEPLOYMENT_DIR%"

ECHO ===========================================================================
ECHO Copy the library files
ECHO ===========================================================================

REM QtTabToolbar
COPY "%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar\src\TabToolbar\Release\TabToolbar.dll" "%ACI_DEPLOYMENT_DIR%"

REM UI Core
COPY "%UI_CORE_ROOT%\x64\Release\uiCore.dll" "%ACI_DEPLOYMENT_DIR%"

REM Curl
COPY "%UI_CORE_ROOT%\Third_Party_Libraries\curl\bin\libcurl.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%UI_CORE_ROOT%\Third_Party_Libraries\curl\bin\zlib1.dll" "%ACI_DEPLOYMENT_DIR%"

REM Qt
COPY "%QTDIR_ROOT%\bin\libEGL.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\libGLESv2.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\opengl32sw.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DAnimation.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DCore.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DExtras.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DInput.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DLogic.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuick.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickAnimation.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickExtras.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickInput.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickRender.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickScene2D.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DRender.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Bluetooth.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Charts.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Concurrent.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Core.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5DataVisualization.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5DBus.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Gamepad.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Gui.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Help.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Location.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Multimedia.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5MultimediaQuick.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5MultimediaWidgets.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Network.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5NetworkAuth.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Nfc.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5OpenGL.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Positioning.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5PositioningQuick.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5PrintSupport.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Purchasing.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Qml.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Quick.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickControls2.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickParticles.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickTemplates2.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickTest.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickWidgets.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5RemoteObjects.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Script.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5ScriptTools.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Scxml.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Sensors.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5SerialBus.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5SerialPort.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Sql.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Svg.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Test.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5TextToSpeech.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebChannel.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebEngine.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebEngineCore.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebEngineWidgets.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebSockets.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebView.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Widgets.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WinExtras.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Xml.dll" "%ACI_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5XmlPatterns.dll" "%ACI_DEPLOYMENT_DIR%"

MKDIR "%ACI_DEPLOYMENT_DIR%\plugins"
XCOPY /S "%QTDIR_ROOT%\plugins" "%ACI_DEPLOYMENT_DIR%\plugins"

REM Copy the icons

MKDIR "%ACI_DEPLOYMENT_DIR%\icons"
XCOPY /S "%ACI_ROOT%\Icons" "%ACI_DEPLOYMENT_DIR%\icons"

REM Copy the Application
COPY "%ACI_UI%\x64\Release\aci.exe" "%ACI_DEPLOYMENT_DIR%"
COPY "%ACI_CORE%\x64\Release\aciCore.dll" "%ACI_DEPLOYMENT_DIR%"

MKDIR "%ACI_DEPLOYMENT_DIR%\Scripts"
XCOPY /S "%ACI_ROOT%\Scripts" "%ACI_DEPLOYMENT_DIR%\Scripts"

ECHO [Paths] > "%ACI_DEPLOYMENT_DIR%\qt.conf"
ECHO Plugins = .\\plugins >> "%ACI_DEPLOYMENT_DIR%\qt.conf"

:END