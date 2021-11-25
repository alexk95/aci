#include <aci/InterpreterObject.h>
#include <aci/AbstractPrinter.h>
#include <aci/aFile.h>
#include <aci/aDir.h>
#include <aci/OS.h>
#include <aci/Convert.h>
#include <aci/AbstractInterpreterNotifier.h>

aci::InterpreterObject::InterpreterObject() : m_printer(nullptr), m_core(nullptr), m_notifier(nullptr) {}

// ################################################################################################################################

// Output

void aci::InterpreterObject::showInfo(void) {
	showDelimiterLine();
	print("###  ");
	print(key());
	print("  ###\n\n");
	showCommandInfo();
}

void aci::InterpreterObject::showDelimiterLine(void) {
	print("##################################################\n");
}

// ################################################################################################################################

// File operations

bool aci::InterpreterObject::readDataFile(std::wstring& _data, const std::wstring& _filename, bool _showLog) {
	std::wstring filePath = OS::instance()->handler()->scriptDirectory() + L"/" + key() + L"/" + _filename;
	return OS::instance()->handler()->readFile(_data, filePath);
}

bool aci::InterpreterObject::readDataFile(std::list<std::wstring>& _data, const std::wstring& _filename, bool _showLog) {
	std::wstring filePath = OS::instance()->handler()->scriptDirectory() + L"/" + key() + L"/" + _filename;
	return OS::instance()->handler()->readLinesFromFile(_data, filePath);
}

bool aci::InterpreterObject::writeDataFile(const std::wstring& _data, const std::wstring& _filename, bool _showLog) {
	std::wstring filePath = OS::instance()->handler()->scriptDirectory() + L"/" + key() + L"/" + _filename;
	for (size_t i{ 0 }; i < filePath.length(); i++) {
		if (filePath.at(i) == L'\\') { filePath.replace(i, 1, L"/"); }
	}
	return OS::instance()->handler()->writeFile(_data, filePath);
}

bool aci::InterpreterObject::writeDataFile(const std::list<std::wstring>& _data, const std::wstring& _filename, bool _showLog) {
	std::wstring filePath = OS::instance()->handler()->scriptDirectory() + L"/" + key() + L"/" + _filename;
	for (size_t i{ 0 }; i < filePath.length(); i++) {
		if (filePath.at(i) == L'\\') { filePath.replace(i, 1, L"/"); }
	}
	return OS::instance()->handler()->writeLinesToFile(_data, filePath);
}

// ################################################################################################################################

// Setter

void aci::InterpreterObject::disableInput(void) {
	if (m_notifier) {
		m_notifier->disableInput();
	}
}

void aci::InterpreterObject::queueDisableInput(void) {
	if (m_notifier) { m_notifier->disableInputAsync(); }
}

void aci::InterpreterObject::enableInput(void) {
	if (m_notifier) {
		m_notifier->enableInput();
	}
}

void aci::InterpreterObject::queueEnableInput(void) {
	if (m_notifier) { m_notifier->enableInputAsync(); }
}

void aci::InterpreterObject::print(const char * _message) { print(std::string(_message)); }
void aci::InterpreterObject::print(const wchar_t * _message) { print(std::wstring(_message)); }
void aci::InterpreterObject::queuePrint(const char * _message) { queuePrint(std::string(_message)); }
void aci::InterpreterObject::queuePrint(const wchar_t * _message) { queuePrint(std::wstring(_message)); }


void aci::InterpreterObject::print(bool _value) {
	if (_value) { print(L"TRUE"); }
	else { print(L"FALSE"); }
}
void aci::InterpreterObject::queuePrint(bool _value) {
	if (_value) { queuePrint(L"TRUE"); }
	else { queuePrint(L"FALSE"); }
}

void aci::InterpreterObject::print(const std::string& _message) {
	if (m_printer) { m_printer->print(_message); }
}
void aci::InterpreterObject::print(const std::wstring& _message) {
	if (m_printer) { m_printer->print(_message); }
}
void aci::InterpreterObject::queuePrint(const std::string& _message) {
	if (m_printer) { m_printer->printAsync(_message); }
}
void aci::InterpreterObject::queuePrint(const std::wstring& _message) {
	if (m_printer) { m_printer->printAsync(_message); }
}

void aci::InterpreterObject::setColor(int _r, int _g, int _b, int _a) {
	setColor(Color(_r, _g, _b, _a));
}
void aci::InterpreterObject::queueColor(int _r, int _g, int _b, int _a) { queueColor(Color(_r, _g, _b, _a)); }

void aci::InterpreterObject::setColor(const Color& _color) {
	if (m_printer) { m_printer->setColor(_color); }
}
void aci::InterpreterObject::queueColor(const Color& _color) {
	if (m_printer) { m_printer->setColorAsync(_color); }
}

// ################################################################################################################################

// Getter

std::list<std::wstring> aci::InterpreterObject::filesInDirectory(const std::wstring& _subdirectory) {
	AbstractOSHandler * os = OS::instance()->handler();
	return os->filesInDirectory(_subdirectory, true);
}

std::list<std::wstring> aci::InterpreterObject::subdirectories(const std::wstring& _subdirectory) {
	AbstractOSHandler * os = OS::instance()->handler();
	return os->subdirectories(_subdirectory, true);
}

// ################################################################################################################################

// Static functions

std::wstring aci::InterpreterObject::isolateFilename(const std::wstring& _path) {
	std::wstring path = _path;
	for (size_t i{ 0 }; i < path.length(); i++) {
		if (path.at(i) == L'\\') { path.replace(i, 1, L"/"); }
	}
	std::list<std::wstring> lst = splitString(path, L"/");
	std::list<std::wstring> filename = splitString(lst.back(), L".");
	std::wstring file;
	for (auto f : filename) {
		if (!file.empty()) { file.append(L"."); }
		file.append(f);
	}
	return file;
}

