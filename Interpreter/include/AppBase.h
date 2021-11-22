#pragma once

#include <aci/AbstractPrinter.h>
#include "AbstractInterpreterNotifier.h"

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
class AppBase : public QObject, public ak::aWindowEventHandler, public aci::AbstractPrinter, public AbstractInterpreterNotifier
{
	Q_OBJECT
public:
	//! @brief Constructor
	//! This constructor is creating all the objects and registering all callbacks
	AppBase(int _argc, char ** _argv);

	//! @brief Deconstructor
	virtual ~AppBase();
	
	//! @brief This function will be called when the window is about to close
	//! If return false then the window will not be closed
	virtual bool closeEvent(void) override;

	virtual void setColor(const QColor& _color) override;

	virtual void print(const QString& _str) override;

	virtual void shutdown(void) override;

	void loadScripts(void);

	void setInputEnabled(bool _isEnabled);

private slots:
	void slotHandle(void);

	void slotPrintMessage(const QString& _message);

	void slotKeyDownOnInpout(QKeyEvent * _event);

	void slotKeyUpOnInput(QKeyEvent * _event);

	void slotTabPressOnInput(void);

	void slotInputChanged(const QString& _text);

private:
	friend class TabToolbar;

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