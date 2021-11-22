// Application header
#include "AppBase.h"			// Corresponding header
#include "InterpreterCore.h"
#include <aci/Settings.h>
#include <aDir.h>
#include <aFile.h>
#include "Windows.h"

// Scripts
#include "sCode.h"
#include "sMerge.h"
#include "sRM.h"

// AK header
#include <akAPI/uiAPI.h>		// The uiAPI
#include <akGui/aColorStyle.h>	// ColorStyle if needed for custom widgets
#include <akWidgets/aWindow.h>
#include <akWidgets/aWindowManager.h>
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aTabWidget.h>
#include <akWidgets/aLabelWidget.h>

#include <qwidget.h>
#include <qlayout.h>

// Qt header
#include <qevent.h>

using namespace ak;
typedef aci::InterpreterObject ** (__stdcall *f_generateObjects)(int& _count);

// We create some constant values for the icons
const QString c_dialogErrorIcon = "DialogError";
const QString c_dialogIconPath = "Dialog";

const std::string c_appVersion = "1.0";
const QString c_settingsWindowState = "WindowState";
const QString c_settingsColorStyle = "ColorStyle";

AppBase::AppBase(int _argc, char ** _argv)
{
	// Create own UID
	m_uid = ak::uiAPI::createUid();

	// Create the main window
	UID window = uiAPI::createWindow(m_uid);
	m_mainWindow = uiAPI::object::get<aWindowManager>(window);
	m_mainWindow->window()->setWindowIcon(uiAPI::getIcon("aci", "Window"));

	// Create layouts
	m_centralLayoutW = new QWidget;
	m_centralLayout = new QVBoxLayout(m_centralLayoutW);

	m_inLayoutW = new QWidget;
	m_inLayout = new QHBoxLayout(m_inLayoutW);
	m_inLayout->setContentsMargins(0, 0, 0, 0);

	// Create controls
	m_tabView = new aTabWidget;
	//m_tabView->setTabOrder(nullptr, nullptr);

	m_out = new aTextEditWidget;
	m_out->setReadOnly(true);
	m_out->setAutoScrollToBottom(true);
	m_out->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	m_out->setDisabled(true);

	//m_out->setTabOrder(nullptr, nullptr);
	{
		QFont f = m_out->font();
		f.setFamily("Courier");
		f.setFixedPitch(true);
		m_out->setFont(f);
	}

	m_in = new aLineEditWidget;
	m_in->setIgnoreTabulator(true);
	m_in->setObjectName("a_input");
	m_inLabel = new aLabelWidget("<:");
	m_inLabel->setObjectName("a_inputLabel");

	// Setup layouts
	m_tabView->addTab(m_out, "Output");
	m_inLayout->addWidget(m_inLabel, 0);
	m_inLayout->addWidget(m_in, 1);

	m_centralLayout->addWidget(m_tabView);
	m_centralLayout->addWidget(m_inLayoutW);

	m_mainWindow->setCentralWidget(m_centralLayoutW);

	// Show window
	m_mainWindow->addEventHandler(this);

	if (uiAPI::settings::getBool("Fullscreen", false)) {
		m_mainWindow->showMaximized();
	}
	else {
		m_mainWindow->showNormal();
	}

	// Coloring
	uiAPI::addPaintable(m_tabView);
	uiAPI::addPaintable(m_out);
	uiAPI::addPaintable(m_in);
	uiAPI::addPaintable(m_inLabel);
	uiAPI::setDefaultDarkColorStyle();

	m_inLabel->setStyleSheet("#a_inputLabel{color: #40ff40;}");

	// Load application settings
	aci::Settings::instance();

	// Start interpreter
	loadScripts();
	InterpreterCore::instance()->attachPrinter(this);
	InterpreterCore::instance()->attachNotifier(this);

	m_inLabel->setText("<: aci@" + InterpreterCore::instance()->currentPath() + " <: ");

	setColor(QColor(150, 150, 255));
	print("<: aci@" + InterpreterCore::instance()->currentPath() + " <: ");
	setColor(QColor(255, 255, 255));
	print("Use \"?\" for help\n");

	connect(m_in, &aLineEditWidget::returnPressed, this, &AppBase::slotHandle);
	connect(m_in, &aLineEditWidget::textChanged, this, &AppBase::slotInputChanged);
	connect(m_in, &aLineEditWidget::keyPressed, this, &AppBase::slotKeyDownOnInpout);
	connect(m_in, &aLineEditWidget::keyReleased, this, &AppBase::slotKeyUpOnInput);
	connect(m_in, &aLineEditWidget::tabPressed, this, &AppBase::slotTabPressOnInput);
	m_in->setFocus();
}

AppBase::~AppBase() {
	InterpreterCore::clearInstance();
}

bool AppBase::closeEvent(void) {
	uiAPI::settings::setBool("Fullscreen", m_mainWindow->window()->isMaximized());
	return true;
}

void AppBase::setColor(const QColor& _color) {
	m_out->moveCursor(QTextCursor::End);
	QTextCursor cursor(m_out->textCursor());

	QTextCharFormat format = cursor.charFormat();
	format.setForeground(QBrush(_color));
	cursor.setCharFormat(format);

	m_out->setTextCursor(cursor);
}

void AppBase::print(const QString& _str) {
	QTextCursor cursor(m_out->textCursor());
	cursor.insertText(_str);
}

void AppBase::shutdown(void) {
	m_mainWindow->close();
}

void AppBase::loadScripts(void) {
	InterpreterCore * i = InterpreterCore::instance();
	i->addScriptObject(aci::Settings::instance());
	i->addScriptObject(new sCode);
	i->addScriptObject(new sMerge(this));
	i->addScriptObject(new sRM);

	// Load custom scripts
	aci::aDir dir("", aci::Settings::instance()->dataPath() + "/Scripts");
	dir.scanFiles(false);
	dir.filterFilesWithWhitelist({ ".dll" });

	for (auto f : dir.files()) {
		setColor(QColor(255, 255, 255));
		print("Load library: " + f->name());
		HINSTANCE hGetProcIDDLL = LoadLibrary(f->fullPath().toStdWString().c_str());

		if (hGetProcIDDLL) {
			// resolve function address here
			f_generateObjects func = (f_generateObjects)GetProcAddress(hGetProcIDDLL, "generateObjects");
			if (func) {
				int objCt{ 0 };
				aci::InterpreterObject ** obj = func(objCt);
				int ct{ 0 };
				for (int o{ 0 }; o < objCt; o++) {
					if (obj[o]) {
						i->addScriptObject(obj[o]);
						ct++;
					}
				}

				if (ct == 0) {
					setColor(QColor(255, 150, 50));
					print("  DONE");
					setColor(QColor(255, 255, 255));
					print(" (no commands loaded)\n");
				}
				else {
					setColor(QColor(0, 255, 0));
					print("  SUCCESS");
					setColor(QColor(255, 255, 255));
					print(" (" + QString::number(ct) + " commands loaded)\n");
				}
			}
			else {
				setColor(QColor(255, 0, 0));
				print("  FAILED (Entry point not found)\n");
			}
		}
		else {
			setColor(QColor(255, 0, 0));
			print("  FAILED (dll not loaded)\n");
		}
	}
}

void AppBase::setInputEnabled(bool _isEnabled) {
	m_in->setEnabled(_isEnabled);
	if (_isEnabled) {
		m_inLabel->setStyleSheet("#a_inputLabel{color: #40ff40;}");
		m_in->setFocus();
	}
	else {
		m_inLabel->setStyleSheet("#a_inputLabel{color: #ff4040;}");
	}
}

// #########################################################################################

// Slots

void AppBase::slotHandle(void) {
	if (m_in->text().isEmpty()) { return; }
	QString cmd = m_in->text();
	
	if (cmd == "clear") {
		m_out->clear();
		setColor(QColor(255, 255, 255));
	}
	else {
		setColor(QColor(150, 150, 255));
		print("\n<: aci@" + InterpreterCore::instance()->currentPath() + " <: ");
		setColor(QColor(255, 255, 255));
		print(cmd + "\n");
		InterpreterCore::instance()->handle(cmd);
	}

	m_in->clear();
	m_inLabel->setText("<: aci@" + InterpreterCore::instance()->currentPath() + " <: ");
	m_in->setFocus();
	m_commandBuffer.push_back(cmd);
	m_commandIndex = m_commandBuffer.size() + 1;
}

void AppBase::slotPrintMessage(const QString& _message) {
	QTextCursor cursor(m_out->textCursor());
	m_out->append(_message);
}

void AppBase::slotKeyDownOnInpout(QKeyEvent * _event) {
	if (_event->key() == Qt::Key_Up) {
		if (m_commandIndex == 0) { return; }
		else {
			m_commandIndex--;
			if (m_commandIndex == 0 || m_commandBuffer.empty()) { m_in->setText(""); }
			else { m_in->setText(m_commandBuffer[m_commandIndex - 1]); }
		}
		
	}
	else if (_event->key() == Qt::Key_Down) {
		if (m_commandIndex > m_commandBuffer.size()) { return; }
		m_commandIndex++;
		if (m_commandIndex == 0) { m_in->setText(""); }
		else if (m_commandIndex > m_commandBuffer.size()) {
			m_commandIndex = m_commandBuffer.size() + 1;
			m_in->setText("");
		}
		else { m_in->setText(m_commandBuffer[m_commandIndex - 1]); }
	}
}

void AppBase::slotKeyUpOnInput(QKeyEvent * _event) {

}

void AppBase::slotTabPressOnInput(void) {
	if (m_lastCommand.isEmpty()) { return; }
	
	// Get input
	QStringList lst;
	InterpreterCore::instance()->extractClassicSyntax(lst, m_lastCommand);

	if (lst.size() == 0) { return; }
	aci::InterpreterObject * item = InterpreterCore::instance()->findFirstMatchingItem(lst.front());

	if (item) {
		if (lst.count() > 1) {
			
		}
	}
}

void AppBase::slotInputChanged(const QString& _text) {
	m_lastCommand = _text;
}
