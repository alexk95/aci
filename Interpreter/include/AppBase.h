#pragma once

#include <aci/AbstractPrinter.h>
#include <aci/AbstractInterpreterNotifier.h>
#include <aci/AbstractOSHandler.h>

// uiCore header
#include <akCore/globalDataTypes.h>		// UID type
#include <akGui/aWindowEventHandler.h>	// Window event notifications (close event)

// QT header
#include <qobject.h>
#include <qcolor.h>

// C++ header
#include <string>

// Forward declarations
class TabToolbar;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QKeyEvent;

namespace ak {
	class aLineEditWidget;
	class aTextEditWidget;
	class aTabWidget;
	class aLabelWidget;
	class aWindowManager;
}

//! This class is managing the UI
//! We derive from aWindowEventHandler to be able to register this class as a
//! window event handler for the main window
class AppBase : public QObject, public ak::aWindowEventHandler, public aci::AbstractPrinter, public aci::AbstractInterpreterNotifier, public aci::AbstractOSHandler
{
	Q_OBJECT
public:
	//! @brief Constructor
	//! This constructor is creating all the objects and registering all callbacks
	AppBase(int _argc, char ** _argv);

	//! @brief Deconstructor
	virtual ~AppBase();
	
	// ##################################################################################################

	// Base class functions

	//! @brief This function will be called when the window is about to close
	//! If return false then the window will not be closed
	virtual bool closeEvent(void) override;

	virtual void disableInput(void) override;
	virtual void enableInput(void) override;

	virtual void shutdown(void) override;

	virtual void print(const std::string& _str) override;
	virtual void print(const std::wstring& _str) override;
	void print(const QString& _str);

	virtual void setColor(const aci::Color& _color) override;
	void setColor(const QColor& _color);

	virtual bool fileExists(const std::wstring& _path) override;
	virtual bool deleteFile(const std::wstring& _path) override;
	virtual bool readFile(std::wstring& _data, const std::wstring& _path) override;
	virtual bool editFile(std::list<std::wstring>& _data, std::wstring& _path) override;
	virtual bool readLinesFromFile(std::list<std::wstring>& _data, const std::wstring& _path) override;
	virtual bool writeFile(const std::wstring& _data, const std::wstring& _path) override;
	virtual bool writeLinesToFile(const std::list<std::wstring>& _data, const std::wstring& _path) override;

	virtual bool directoryExists(const std::wstring& _path) override;
	virtual std::list<std::wstring> filesInDirectory(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) override;
	virtual std::list<std::wstring> subdirectories(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) override;
	virtual std::wstring currentDirectory(void) override;
	virtual std::wstring scriptDataDirectory(void) override;

	virtual std::wstring getSettingsValue(const std::string& _key, const std::wstring& _defaultValue = std::wstring()) override;
	virtual void setSettingsValue(const std::string& _key, const std::wstring& _value) override;
	
	//! @brief Will request the next command from the Interpreter
	//! The request will be queued. When the user has finished the input the command will be send to the proivided object
	//! @param _obj The object the input will be send to
	virtual void requestNextCommand(aci::InterpreterObject * _obj) override;
	
	// ##################################################################################################

	void loadScripts(void);

private slots:
	void slotHandle(void);

	void slotSetInputEnabled(bool _enabled);
	void slotPrintMessage(const QString& _message);
	void slotSetColor(const QColor& _color);

	void slotKeyDownOnInpout(QKeyEvent * _event);

	void slotKeyUpOnInput(QKeyEvent * _event);

	void slotTabPressOnInput(void);

	void slotInputChanged(const QString& _text);

private:
	friend class TabToolbar;

	bool queueRequired(void);

	ak::UID					m_uid;			//! The UID of this class, used to create controls

	QWidget *				m_centralLayoutW;
	QVBoxLayout *			m_centralLayout;

	QWidget *				m_inLayoutW;
	QHBoxLayout *			m_inLayout;

	ak::aTabWidget *		m_tabView;
	ak::aTextEditWidget *	m_out;
	ak::aLineEditWidget *	m_in;
	ak::aLabelWidget *		m_inLabel;
	ak::aWindowManager *	m_mainWindow;

	std::vector<QString>	m_commandBuffer;
	size_t					m_commandIndex;
	QString					m_lastCommand;

	AppBase() = delete;
	AppBase(AppBase &) = delete;
	AppBase& operator = (AppBase&) = delete;
};