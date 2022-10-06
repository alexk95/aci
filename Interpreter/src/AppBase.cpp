// Application header
#include "AppBase.h"			// Corresponding header
#include "Settings.h"
#include "Windows.h"

#include <aci/API.h>
#include <aci/InterpreterCore.h>
#include <aci/aDir.h>
#include <aci/aFile.h>
#include <aci/ScriptLoader.h>

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
#include <qtextstream.h>
#include <qdir.h>
#include <qfile.h>

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

	print(L"Initializing...\n");
	print(L"UI starting at: " + QDir::currentPath().toStdWString() + L"\n");

	// Start interpreter
	print(L"Initialize interpreter\n");
	aci::API::initialize(this, this, this);
	print(L"Update data path\n\n");
	aci::API::setCurrentPath(Settings::instance()->dataPath());
	print(L"Load external scripts\n");
	loadScripts();

	print(L"\nWelcome\n");

	m_inLabel->setText("<: aci@" + QString::fromStdWString(aci::API::currentPath()) + " <: ");

	setColor(aci::Color(150, 150, 255));
	print("<: aci@" + QString::fromStdWString(aci::API::currentPath()) + " <: ");
	setColor(QColor(255, 255, 255));
	print(QString("Use \"Welcome\n"));

	setColor(aci::Color(150, 150, 255));
	print("<: aci@" + QString::fromStdWString(aci::API::currentPath()) + " <: ");
	setColor(QColor(255, 255, 255));
	print(QString("Use \"?\" for help\n"));

	connect(m_in, &aLineEditWidget::returnPressed, this, &AppBase::slotHandle);
	connect(m_in, &aLineEditWidget::textChanged, this, &AppBase::slotInputChanged);
	connect(m_in, &aLineEditWidget::keyPressed, this, &AppBase::slotKeyDownOnInpout);
	connect(m_in, &aLineEditWidget::keyReleased, this, &AppBase::slotKeyUpOnInput);
	connect(m_in, &aLineEditWidget::tabPressed, this, &AppBase::slotTabPressOnInput);
	m_in->setFocus();
}

AppBase::~AppBase() {
	aci::API::cleanUp();
}

// #########################################################################################

// Base class functions

bool AppBase::closeEvent(void) {
	uiAPI::settings::setBool("Fullscreen", m_mainWindow->window()->isMaximized());
	return true;
}

void AppBase::disableInput(void) {
	setInputEnabled(false);
}
void AppBase::disableInputAsync(void) {
	QMetaObject::invokeMethod(this, "slotSetInputEnabled", Qt::QueuedConnection, Q_ARG(bool, false));
}
void AppBase::enableInput(void) {
	setInputEnabled(true);
}
void AppBase::enableInputAsync(void) {
	QMetaObject::invokeMethod(this, "slotSetInputEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
}

void AppBase::shutdown(void) {
	m_mainWindow->close();
}

void AppBase::print(const std::string& _str) { slotPrintMessage(QString(_str.c_str())); }
void AppBase::print(const std::wstring& _str) { slotPrintMessage(QString::fromWCharArray(_str.c_str())); }
void AppBase::print(const QString& _str) { slotPrintMessage(_str); }
void AppBase::printAsync(const std::string& _str) { QMetaObject::invokeMethod(this, "slotPrintMessage", Qt::QueuedConnection, Q_ARG(const QString&, QString(_str.c_str()))); }
void AppBase::printAsync(const std::wstring& _str) { QMetaObject::invokeMethod(this, "slotPrintMessage", Qt::QueuedConnection, Q_ARG(const QString&, QString::fromWCharArray(_str.c_str()))); }
void AppBase::printAsync(const QString& _str) { QMetaObject::invokeMethod(this, "slotPrintMessage", Qt::QueuedConnection, Q_ARG(const QString&, _str)); }
void AppBase::setColor(const aci::Color& _color) { slotSetColor(QColor(_color.r(), _color.g(), _color.b(), _color.a())); }
void AppBase::setColorAsync(const aci::Color& _color) { QMetaObject::invokeMethod(this, "slotSetColor", Qt::QueuedConnection, Q_ARG(const QColor&, QColor(_color.r(), _color.g(), _color.b(), _color.a()))); }
void AppBase::setColor(const QColor& _color) { slotSetColor(_color); }
void AppBase::setColorAsync(const QColor& _color) { QMetaObject::invokeMethod(this, "slotSetColor", Qt::QueuedConnection, Q_ARG(const QColor&, _color)); }

bool AppBase::fileExists(const std::wstring& _path) {
	return QFile(QString::fromStdWString(_path)).exists();
}

bool AppBase::deleteFile(const std::wstring& _path) {
	QFile file(QString::fromStdWString(_path));
	if (file.exists()) {
		return file.remove();
	}
	return true;
}

bool AppBase::readFile(std::wstring& _data, const std::wstring& _path) {
	QFile file(QString::fromStdWString(_path));
	if (!file.exists()) {
		assert(0);
		return false;
	}
	if (!file.open(QIODevice::ReadOnly)) { assert(0); return false; }
	_data = QString(file.readAll()).toStdWString();
	file.close();
	return true;
}

bool AppBase::editFile(std::list<std::wstring>& _data, std::wstring& _path) {
	return false;
}

bool AppBase::readLinesFromFile(std::list<std::wstring>& _data, const std::wstring& _path) {
	QFile file(QString::fromStdWString(_path));
	if (!file.exists()) { assert(0); return false; }
	if (!file.open(QIODevice::ReadOnly)) { assert(0); return false; }
	while (!file.atEnd()) {
		_data.push_back(QString(file.readLine()).toStdWString());
	}

	file.close();
	return true;
}

bool AppBase::writeFile(const std::wstring& _data, const std::wstring& _path) {
	QStringList lst = QString::fromStdWString(_path).replace("\\", "/").split("/");
	QString dir;
	for (int p{ 0 }; p < lst.count() - 1; p++) {
		if (!dir.isEmpty()) { dir.append("/"); }
		dir.append(lst[p]);
		if (!QDir(dir).exists()) {
			if (!QDir().mkdir(dir)) { assert(0); return false; }
		}
	}

	QFile file(QString::fromStdWString(_path));
	if (!file.open(QIODevice::WriteOnly)) { assert(0); return false; }
	file.write(QByteArray().append(QString::fromStdWString(_data)));
	file.close();
	return true;
}

bool AppBase::writeLinesToFile(const std::list<std::wstring>& _data, const std::wstring& _path) {
	QStringList lst = QString::fromStdWString(_path).replace("\\", "/").split("/");
	QString dir;
	for (int p{ 0 }; p < lst.count() - 1; p++) {
		if (!dir.isEmpty()) { dir.append("/"); }
		dir.append(lst[p]);
		if (!QDir(dir).exists()) {
			if (!QDir().mkdir(dir)) { assert(0); return false; }
		}
	}

	QFile file(QString::fromStdWString(_path));
	if (!file.open(QIODevice::WriteOnly)) { assert(0); return false; }
	QTextStream stream(&file);
	for (auto l : _data) {
		stream << QString::fromStdWString(l) << "\n";
	}
	file.close();
	return true;
}

bool AppBase::directoryExists(const std::wstring& _path) {
	return QDir(QString::fromStdWString(_path)).exists();
}

std::list<std::wstring> AppBase::filesInDirectory(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) {
	QDir dir(QString::fromStdWString(_path));
	std::list<std::wstring> ret;
	if (dir.exists()) {
		for (auto entry : dir.entryList(QDir::Filter::Files)) {
			if (entry != "." && entry != "..") {
				ret.push_back(entry.toStdWString());
			}
		}
	}
	return ret;
}

std::list<std::wstring> AppBase::subdirectories(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) {
	QDir dir(QString::fromStdWString(_path));
	std::list<std::wstring> ret;
	if (dir.exists()) {
		for (auto entry : dir.entryList(QDir::Filter::Dirs)) {
			if (entry != "." && entry != "..") {
				ret.push_back(entry.toStdWString());
			}
		}
	}
	return ret;
}

std::wstring AppBase::currentDirectory(void) {
	return QDir::currentPath().toStdWString();
}

std::wstring AppBase::scriptDataDirectory(void) {
	return Settings::instance()->dataPath() + L"/ScriptData";
}

std::wstring AppBase::getSettingsValue(const std::string& _key, const std::wstring& _defaultValue) {
	return uiAPI::settings::getString(QString::fromStdString(_key), QString::fromStdWString(_defaultValue)).toStdWString();
}

void AppBase::setSettingsValue(const std::string& _key, const std::wstring& _value) {
	uiAPI::settings::setString(QString::fromStdString(_key), QString::fromStdWString(_value));
}

void AppBase::requestNextCommand(aci::InterpreterObject * _obj) {

}

// #########################################################################################

void AppBase::loadScripts(void) {
	aci::InterpreterCore * i = aci::API::core();
	print(L"Adding settings as a script\n");
	i->addScriptObject(Settings::instance());
#ifdef _DEBUG
	print(L"Load *.dll scripts from " + Settings::instance()->dataPath() + L"/ScriptsD\n");
	aci::API::core()->scriptLoader()->loadDllsFromDirectory(Settings::instance()->dataPath() + L"/ScriptsD");
#else
	print(L"Load *.dll scripts from " + Settings::instance()->dataPath() + L"/Scripts\n");
	aci::API::core()->scriptLoader()->loadDllsFromDirectory(Settings::instance()->dataPath() + L"/Scripts");
#endif // _DEBUG


	//print(L"Load merge script\n");
	//aci::API::core()->scriptLoader()->loadDllsFromDirectory(QString(qgetenv("ACI_DEFAULT_MERGE") + "\\x64\\Debug\\").toStdWString());
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
		print(L"\n<: aci@" + aci::API::currentPath() + L" <: ");
		setColor(QColor(255, 255, 255));
		print(cmd + "\n");
		aci::API::core()->handle(cmd.toStdWString());
	}

	m_in->clear();
	m_inLabel->setText("<: aci@" + QString::fromStdWString(aci::API::core()->currentPath()) + " <: ");
	m_in->setFocus();
	m_commandBuffer.push_back(cmd);
	m_commandIndex = m_commandBuffer.size() + 1;
}

void AppBase::slotSetInputEnabled(bool _enabled) {
	setInputEnabled(_enabled);
}

void AppBase::slotPrintMessage(const QString& _message) {
	QTextCursor cursor(m_out->textCursor());
	//m_out->append(_message);
	cursor.insertText(_message);
}

void AppBase::slotSetColor(const QColor& _color) {
	m_out->moveCursor(QTextCursor::End);
	QTextCursor cursor(m_out->textCursor());

	QTextCharFormat format = cursor.charFormat();
	format.setForeground(QBrush(_color));
	cursor.setCharFormat(format);

	m_out->setTextCursor(cursor);
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
	else if (_event->key() == Qt::Key_Alt) {
		m_out->setEnabled(true);
	}
}

void AppBase::slotKeyUpOnInput(QKeyEvent * _event) {
	if (_event->key() == Qt::Key_Alt) {
		m_out->setEnabled(false);
	}
}

void AppBase::slotTabPressOnInput(void) {
	if (m_lastCommand.isEmpty()) { return; }
	
	// Get input
	std::vector<std::wstring> lst;
	aci::API::core()->extractClassicSyntax(lst, m_lastCommand.toStdWString());

	if (lst.size() == 0) { return; }
	aci::InterpreterObject * item = aci::API::core()->findFirstMatchingItem(lst.front());

	if (item) {
		if (lst.size() > 1) {
			
		}
	}
}

void AppBase::slotInputChanged(const QString& _text) {
	m_lastCommand = _text;
}
