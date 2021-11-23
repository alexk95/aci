#include <aci/InterpreterObject.h>
#include <aci/AbstractPrinter.h>
#include <aci/Settings.h>
#include <aci/aFile.h>
#include <aci/aDir.h>
#include <aci/OS.h>
#include <aci/Convert.h>

aci::InterpreterObject::InterpreterObject() : m_printer(nullptr) {}

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

bool aci::InterpreterObject::readDataFile(std::list<std::wstring>& _data, const std::wstring& _filename, bool _showLog) {
	std::wstring filePath = Settings::instance()->dataPath() + L"/ScriptData/" + key() + L"/" + _filename;
	return OS::instance()->handler()->readLinesFromFile(_data, filePath);
}

bool aci::InterpreterObject::writeDataFile(const std::list<std::wstring>& _data, const std::wstring& _filename, bool _showLog) {
	std::wstring filePath = Settings::instance()->dataPath() + L"/ScriptData/" + key() + L"/" + _filename;
	for (size_t i{ 0 }; i < filePath.length(); i++) {
		if (filePath.at(i) == L'\\') { filePath.replace(i, 1, L"/"); }
	}
	return OS::instance()->handler()->writeLinesToFile(_data, filePath);
}

// ################################################################################################################################

// Setter

void aci::InterpreterObject::print(const std::string& _message) {
	if (m_printer) { m_printer->print(_message); }
}

void aci::InterpreterObject::print(const std::wstring& _message) {
	if (m_printer) { m_printer->print(_message); }
}

void aci::InterpreterObject::print(const char * _message) { print(std::string(_message)); }
void aci::InterpreterObject::print(const wchar_t * _message) { print(std::wstring(_message)); }
void aci::InterpreterObject::queuePrint(const char * _message) { queuePrint(std::string(_message)); }
void aci::InterpreterObject::queuePrint(const wchar_t * _message) { queuePrint(std::wstring(_message)); }

void aci::InterpreterObject::queuePrint(bool _value) {
	if (_value) { queuePrint(L"TRUE"); }
	else { queuePrint(L"FALSE"); }
}

void aci::InterpreterObject::queueColor(int _r, int _g, int _b, int _a) { queueColor(Color(_r, _g, _b, _a)); }

// ################################################################################################################################

// Getter



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

void aci::InterpreterObject::setColor(const QColor& _color) {
	if (m_printer) { m_printer->setColor(_color); }
}

void aci::InterpreterObject::setColor(int _r, int _g, int _b, int _a) {
	setColor(QColor(_r, _g, _b, _a));
}

