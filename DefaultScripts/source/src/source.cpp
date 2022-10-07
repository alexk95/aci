#include "source.h"
#include <aci/aFile.h>
#include <aci/Color.h>
#include <AdditionalOperators.h>

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qtextstream.h>

#include <thread>

using namespace aci;

source::source(void)
	: m_searchTopDirectoryOnly(false), m_blackListDActive(false), m_whiteListDActive(false),
	m_blackListFActive(false), m_whiteListFActive(false), m_prefix(L"[source] ")
{}

source::~source() {

}

bool source::handle(const std::wstring& _command, const std::vector<std::wstring>& _params) {
	if (_params.size() == 2) {
		if (_params.back() == L"cfg") { cmdConfiguration(); return true; }
		else if (_params.back() == L"clear") { cmdClear(); return true; }
		else if (_params.back() == L"run") { return run(ALL); }	
		else if (_params.back() == L"dir" || _params.back() == L"directory") { return run(DIRSONLY); }
		else if (_params.back() == L"file") { return run(FILESONLY); }
		else {
			setColor(Color::RED);
			print(m_prefix + L"Invalid arguments for \"source\". Try \"?source\" for help\n");
			return false;
		}
	}
	else if (_params.size() == 3) {
		if (_params[1] == L"wd") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") {
				m_whiteListDActive = true;
				return true;
			}
			else if (opt == "false" || opt == "0") {
				m_whiteListDActive = false;
				return true;
			}
			else {
				setColor(Color::RED);
				print(m_prefix + L"Invalid value for \"Directory whitelist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == L"+wd" || _params[1] == L"wd+") {
			for (auto itm : m_whiteListD) {
				if (itm == _params.back()) { return true; }
			}
			m_whiteListD.push_back(_params.back());
			return true;
		}
		else if (_params[1] == L"-wd" || _params[1] == L"wd-") {
			if (_params.back() == L"*") { m_whiteListD.clear(); return true; }
			for (auto it = m_whiteListD.begin(); it != m_whiteListD.end(); it++) {
				if (*it == _params.back()) {
					m_whiteListD.erase(it);
					return true;
				}
			}
			setColor(Color::RED);
			print(m_prefix + L"Directory whitelist entry \"");
			setColor(Color::ORANGE);
			print(_params.back());
			setColor(Color::RED);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == L"bd") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") {
				m_blackListDActive = true;
				return true;
			}
			else if (opt == "false" || opt == "0") {
				m_blackListDActive = false;
				return true;
			}
			else {
				setColor(Color::RED);
				print(m_prefix + L"Invalid value for \"Directory blacklist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == L"+bd" || _params[1] == L"bd+") {
			for (auto itm : m_blackListD) {
				if (itm == _params.back()) { return true; }
			}
			m_blackListD.push_back(_params.back());
			return true;
		}
		else if (_params[1] == L"-bd" || _params[1] == L"bd-") {
			if (_params.back() == L"*") { m_blackListD.clear(); return true; }
			for (auto it = m_blackListD.begin(); it != m_blackListD.end(); it++) {
				if (*it == _params.back()) {
					m_blackListD.erase(it);
					return true;
				}
			}
			setColor(Color::RED);
			print(m_prefix + L"Directory blacklist entry \"");
			setColor(Color::ORANGE);
			print(_params.back());
			setColor(Color::RED);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == L"wf") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") {
				m_whiteListFActive = true;
				return true;
			}
			else if (opt == "false" || opt == "0") {
				m_whiteListFActive = false;
				return true;
			}
			else {
				setColor(Color::RED);
				print(m_prefix + L"Invalid value for \"File whitelist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == L"+wf" || _params[1] == L"wf+") {
			for (auto itm : m_whiteListF) {
				if (itm == _params.back()) { return true; }
			}
			m_whiteListF.push_back(_params.back());
			return true;
		}
		else if (_params[1] == L"-wf" || _params[1] == L"wf-") {
			if (_params.back() == L"*") { m_whiteListF.clear(); return true; }
			for (auto it = m_whiteListF.begin(); it != m_whiteListF.end(); it++) {
				if (*it == _params.back()) {
					m_whiteListF.erase(it);
					return true;
				}
			}
			setColor(Color::RED);
			print(m_prefix + L"File whitelist entry \"");
			setColor(Color::ORANGE);
			print(_params.back());
			setColor(Color::RED);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == L"bf") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") {
				m_blackListFActive = true;
				return true;
			}
			else if (opt == "false" || opt == "0") {
				m_blackListFActive = false;
				return true;
			}
			else {
				setColor(Color::RED);
				print(m_prefix + L"Invalid value for \"File blacklist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == L"+bf" || _params[1] == L"bf+") {
			for (auto itm : m_blackListF) {
				if (itm == _params.back()) { return true; }
			}
			m_blackListF.push_back(_params.back());
			return true;
		}
		else if (_params[1] == L"-bf" || _params[1] == L"bf+") {
			if (_params.back() == L"*") { m_blackListF.clear(); return true; }
			for (auto it = m_blackListF.begin(); it != m_blackListF.end(); it++) {
				if (*it == _params.back()) {
					m_blackListF.erase(it);
					return true;
				}
			}
			setColor(Color::RED);
			print(m_prefix + L"File blacklist entry \"");
			setColor(Color::ORANGE);
			print(_params.back());
			setColor(Color::RED);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == L"save") {
			return cmdSave(QString::fromStdWString(_params.back()));
		}
		else if (_params[1] == L"load") {
			return cmdLoad(QString::fromStdWString(_params.back()));
		}
		else if (_params[1] == L"dir") {
			m_path = _params.back();
			return true;
		}
		else if (_params[1] == L"lang" || _params[1] == L"language") {
			if (_params[2] == L"c++") m_language = L"c++";
			else m_language = L"";
			return true;
		}
		else if (_params[1] == L"top") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") { m_searchTopDirectoryOnly = true; return true; }
			else if (opt == "false" || opt == "0") { m_searchTopDirectoryOnly = false; return true; }
			else {
				setColor(Color::RED);
				print(m_prefix + L"Invalid value for \"Top level only active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == L"find") {
			return find(_params[2]);
		}
	}

	setColor(Color::RED);
	print(m_prefix + L"Invalid arguments for \"" + key() + L"\"\n");
	return false;
}

// #####################################################################################################

void source::cmdConfiguration(void) {

	setColor(Color::WHITE);
	print(m_prefix);
	printDelimiterLine();
	print(m_prefix + L"###  ");
	setColor(Color::PURPLE);
	print(key() + L" configuration");
	setColor(Color::WHITE);
	print(L"  ###\n");
	print(m_prefix);
	printDelimiterLine();
	
	print(L"\n");
	print(m_prefix + L"Directory:                       ");
	setColor(Color::ORANGE);
	if (m_path.empty()) { print(L"<empty>\n"); }
	else { print(m_path + L"\n"); }
	setColor(Color::WHITE);


	print(m_prefix + L"Search only top level directory: ");
	print(m_searchTopDirectoryOnly);
	setColor(Color::WHITE);
	print("\n");

	print(m_prefix + L"Language:                        ");
	setColor(Color::ORANGE);
	if (m_language.empty()) print("<none>\n");
	else print(m_language + L"\n");
	setColor(Color::WHITE);	

	print(m_prefix + L"Whilelist (directories) active:  ");
	print(m_whiteListDActive);
	setColor(Color::WHITE);
	print("\n");

	print(m_prefix + L"Whitelist (directories):\n");
	setColor(Color::ORANGE);
	if (m_whiteListD.empty()) { print(m_prefix + L"\t<empty>\n"); }
	else {
		for (auto itm : m_whiteListD) { print(m_prefix + L"\t" + itm + L"\n"); }
	}
	setColor(Color::WHITE);

	print(m_prefix + L"Blacklist (directories) active:  ");
	print(m_blackListDActive);
	setColor(Color::WHITE);
	print("\n");

	print(m_prefix + L"Blacklist (directories):\n");
	setColor(Color::ORANGE);
	if (m_blackListD.empty()) { print(m_prefix + L"\t<empty>\n"); }
	else {
		for (auto itm : m_blackListD) { print(m_prefix + L"\t" + itm + L"\n"); }
	}
	setColor(Color::WHITE);

	print(m_prefix + L"Whilelist (files) active:        ");
	print(m_whiteListFActive);
	setColor(Color::WHITE);
	print("\n");

	print(m_prefix + L"Whitelist (files):\n");
	setColor(Color::ORANGE);
	if (m_whiteListF.empty()) { print(m_prefix + L"\t<empty>\n"); }
	else {
		for (auto itm : m_whiteListF) { print(m_prefix + L"\t" + itm + L"\n"); }
	}
	setColor(Color::WHITE);

	print(m_prefix + L"Blacklist (files) active:        ");
	print(m_blackListFActive);
	setColor(Color::WHITE);
	print("\n");

	print(m_prefix + L"Blacklist (files):\n");
	setColor(Color::ORANGE);
	if (m_blackListF.empty()) { print(m_prefix + L"\t<empty>\n"); }
	else {
		for (auto itm : m_blackListF) { print(m_prefix + L"\t" + itm + L"\n"); }
	}
	setColor(Color::WHITE);
}

void source::cmdClear(void) {
	m_blackListDActive = false;
	m_whiteListDActive = false;
	m_blackListFActive = false;
	m_whiteListFActive = false;

	m_blackListD.clear();
	m_whiteListD.clear();
	m_blackListF.clear();
	m_whiteListF.clear();

	m_path.clear();

	m_searchTopDirectoryOnly = false;

	print("[source] Configuration cleared\n");
}

bool source::cmdLoad(const QString& _filename) {
	if (_filename == "?") { showSavedConfigs(); return true; }

	std::wstring data;
	if (!readDataFile(data, ("Configurations/" + _filename + ".json").toStdWString(), true)) {
		return false;
	}

	print("[source] [parse]: Starting parser\n");
	QStringList lst;
	QByteArray byteData;
	byteData.append(QString::fromStdWString(data));

	QJsonDocument doc = QJsonDocument::fromJson(byteData);
	if (!doc.isObject()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Invalid document format\n");
		return false;
	}

	// Check member
	QJsonObject obj = doc.object();
	if (!obj.contains("Directory")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryOne\" is missing\n");
		return false;
	}
	if (!obj.contains("SearchTopOnly")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"SearchTopOnly\" is missing\n");
		return false;
	}
	if (!obj.contains("Language")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"Language\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryWhitelistActive")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryWhitelistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryWhitelist")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryWhitelist\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryBlacklistActive")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryBlacklistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryBlacklist")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryBlacklist\" is missing\n");
		return false;
	}
	if (!obj.contains("FileWhitelistActive")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileWhitelistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("FileWhitelist")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileWhitelist\" is missing\n");
		return false;
	}
	if (!obj.contains("FileBlacklistActive")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileBlacklistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("FileBlacklist")) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileBlacklist\" is missing\n");
		return false;
	}

	// Check data types
	if (!obj["Directory"].isString()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryOne\" is not a string\n");
		return false;
	}
	if (!obj["SearchTopOnly"].isBool()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"SearchTopOnly\" is not a boolean\n");
		return false;
	}
	if (!obj["Language"].isString()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"Language\" is not a string\n");
		return false;
	}
	if (!obj["DirectoryWhitelistActive"].isBool()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryWhitelistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["DirectoryWhitelist"].isArray()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryWhitelist\" is not a array\n");
		return false;
	}
	if (!obj["DirectoryBlacklistActive"].isBool()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryBlacklistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["DirectoryBlacklist"].isArray()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"DirectoryBlacklist\" is not a array\n");
		return false;
	}
	if (!obj["FileWhitelistActive"].isBool()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileWhitelistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["FileWhitelist"].isArray()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileWhitelist\" is not a array\n");
		return false;
	}
	if (!obj["FileBlacklistActive"].isBool()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileBlacklistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["FileBlacklist"].isArray()) {
		print("[source] [parse]: ");
		setColor(Color::RED);
		print("Member \"FileBlacklist\" is not a array\n");
		return false;
	}

	// Collect data
	m_path = obj["Directory"].toString().toStdWString();
	m_searchTopDirectoryOnly = obj["SearchTopOnly"].toBool();
	m_language = obj["Language"].toString().toStdWString();

	m_whiteListDActive = obj["DirectoryWhitelistActive"].toBool();
	m_blackListDActive = obj["DirectoryBlacklistActive"].toBool();
	m_whiteListFActive = obj["FileWhitelistActive"].toBool();
	m_blackListFActive = obj["FileBlacklistActive"].toBool();

	QJsonArray whitelistD = obj["DirectoryWhitelist"].toArray();
	m_whiteListD.clear();
	for (int i{ 0 }; i < whitelistD.count(); i++) {
		if (whitelistD[i].isString()) {
			m_whiteListD.push_back(whitelistD[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @source @load"
		}
	}

	QJsonArray blacklistD = obj["DirectoryBlacklist"].toArray();
	m_blackListD.clear();
	for (int i{ 0 }; i < blacklistD.count(); i++) {
		if (blacklistD[i].isString()) {
			m_blackListD.push_back(blacklistD[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @source @load"
		}
	}

	QJsonArray whitelistF = obj["FileWhitelist"].toArray();
	m_whiteListF.clear();
	for (int i{ 0 }; i < whitelistF.count(); i++) {
		if (whitelistF[i].isString()) {
			m_whiteListF.push_back(whitelistF[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @source @load"
		}
	}

	QJsonArray blacklistF = obj["FileBlacklist"].toArray();
	m_blackListF.clear();
	for (int i{ 0 }; i < blacklistF.count(); i++) {
		if (blacklistF[i].isString()) {
			m_blackListF.push_back(blacklistF[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @source @load"
		}
	}

	print("[source] [parse]: Import successfull\n");

	return true;
}

bool source::cmdSave(const QString& _filename) {
	if (_filename == "?") { showSavedConfigs(); return true; }

	// Collect data
	QJsonObject obj;
	obj["Directory"] = QString::fromStdWString(m_path);
	obj["SearchTopOnly"] = m_searchTopDirectoryOnly;
	obj["Language"] = QString::fromStdWString(m_language);

	obj["DirectoryWhitelistActive"] = m_whiteListDActive;
	QJsonArray whitelistD;
	for (std::wstring itm : m_whiteListD) { whitelistD.push_back(QString::fromStdWString(itm)); }
	obj["DirectoryWhitelist"] = whitelistD;

	obj["DirectoryBlacklistActive"] = m_blackListDActive;
	QJsonArray blacklistD;
	for (std::wstring itm : m_blackListD) { blacklistD.push_back(QString::fromStdWString(itm)); }
	obj["DirectoryBlacklist"] = blacklistD;

	obj["FileWhitelistActive"] = m_whiteListFActive;
	QJsonArray whitelistF;
	for (std::wstring itm : m_whiteListF) { whitelistF.push_back(QString::fromStdWString(itm)); }
	obj["FileWhitelist"] = whitelistF;

	obj["FileBlacklistActive"] = m_blackListFActive;
	QJsonArray blacklistF;
	for (std::wstring itm : m_blackListF) { blacklistF.push_back(QString::fromStdWString(itm)); }
	obj["FileBlacklist"] = blacklistF;

	// Convert data
	QJsonDocument doc(obj);
	QString result;
	QTextStream stream(&result);
	stream << doc.toJson();
	// Write data
	if (writeDataFile(result.toStdWString(), ("Configurations/" + _filename + ".json").toStdWString(), true)) {
		print("[source]: Export successfull\n");
		return true;
	}
	return false;
}

void source::showSavedConfigs(void) {
	std::list<std::wstring> files = filesInDataDirectory(L"/Configurations");
	if (files.empty()) {
		print(m_prefix + L"\t<empty>\n");
	}
	else {
		for (auto entry : files) {
			print(m_prefix + L"\t" + isolateFilename(entry) + L"\n");
		}
	}
}

// #####################################################################################################

bool source::run(RunMode _mode) {

	if (m_path.empty()) {
		setColor(Color::WHITE);
		print(m_prefix);
		setColor(Color::RED);
		print(L"Directory is empty\n");
		return false;
	}

	// Check the directories
	aci::aDir dir(L"", m_path);

	if (!dir.exists()) {
		setColor(Color::WHITE);
		print(m_prefix);
		setColor(Color::RED);
		print(L"The directory \"" + m_path + L"\" does not exist\n");
		return false;
	}

	disableInput();

	std::thread t1(&source::performRun, this, dir, _mode);
	t1.detach();
	return true;
}

bool source::find(const std::wstring& _text) {
	if (m_path.empty()) {
		setColor(Color::WHITE);
		print(m_prefix);
		setColor(Color::RED);
		print(L"Directory is empty\n");
		return false;
	}

	// Check the directories
	aci::aDir dir(L"", m_path);

	if (!dir.exists()) {
		setColor(Color::WHITE);
		print(m_prefix);
		setColor(Color::RED);
		print(L"The directory \"" + m_path + L"\" does not exist\n");
		return false;
	}

	disableInput();

	std::thread t1(&source::performFind, this, dir, _text);
	t1.detach();
	return true;
}

// #####################################################################################################

// Protected functions

void source::showCommandInfo(void) {
	setColor(Color::WHITE);
	print(m_prefix + L"? Source is used to gather some information about a project, like:\n");
	print(m_prefix + L"?   - lines of code\n");
	print(m_prefix + L"?   - number of files\n");

	print(m_prefix + L"\t");
	print(L"cfg               Show the current configuration\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"top <active>      Will activate/disable the top level only option ( true / false / 0 / 1 )\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"lang <language>   Will set the programming language (supported: c++)\n");

	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"load              Show all saved configurations\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"save              Show all saved configurations\n");

	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"run               Will start the source information gathering process with the current configuration\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"dir               Will display all directories that match the current configuration\n");

	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"file              Will display all files that match the current configuration\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"dir <path>        Set directory\n");

	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"files             Will display all files with the current configuration\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"load <filename>   Will load the configuration from the file specified\n"
		"\t                       (");
	setColor(Color::ORANGE);
	print(L"\"../ScriptData/source/Configurations");
	setColor(0, 255, 0);
	print(L"<filename>");
	setColor(Color::ORANGE);
	print(L".json\")\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"save <filename>   Will save the configuration to the file specified\n"
		"\t                       (");
	setColor(Color::ORANGE);
	print(L"\"../ScriptData/source/Configurations");
	setColor(0, 255, 0);
	print(L"<filename>");
	setColor(Color::ORANGE);
	print(L".json\")\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"wd <active>       Will activate/disable the whitelist for directories ( true / false / 0 / 1 )\n");
	
	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"+wd <filter>      Will add the provided filter to the directories whitelist\n");
	setColor(Color::WHITE);
	
	print(m_prefix + L"\t");
	print(L"-wd <filter>      Will remove the provided filter from the directories whitelist\n");
	
	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"-wd *             Will clear the directories whitelist\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"bd <active>       Will activate/disable the blacklist for directories ( true / false / 0 / 1 )\n");
	
	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"+bd <filter>      Will add the provided filter to the directories blacklist\n");
	setColor(Color::WHITE);
	
	print(m_prefix + L"\t");
	print(L"-bd <filter>      Will remove the provided filter from directories the blacklist\n");

	print(m_prefix + L"\t");
	setColor(Color::YELLOW);
	print(L"-bd *             Will clear the directories blacklist\n");
	setColor(Color::WHITE);

	print(m_prefix + L"\t");
	print(L"find <text>       Will scan all files and display the files that contain the given text\n");
}

// #####################################################################################################

// Private functions

void source::performRun(aci::aDir _dir, RunMode _mode) {
	try {

		// Scan

		print(m_prefix + L"Scanning: Directory\n");
		_dir.scanAll();
		
		print(m_prefix + L"Directory information (no filter) loaded with:\n\t- Directories: " +
			QString::number(_dir.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_dir.fileCount()).toStdWString() + L"\n");

		// #########################################################################################################################

		// Apply filter

		if (m_whiteListDActive) {
			print(m_prefix + L"Applying filter: Directory: Whitelist\n");
			_dir.filterDirectoriesWithWhitelist(m_whiteListD);
		}
		if (m_blackListDActive) {
			print(m_prefix + L"Applying filter: Directory: Blacklist\n");
			_dir.filterDirectoriesWithBlacklist(m_blackListD);
		}
		if (m_whiteListFActive) {
			print(m_prefix + L"Applying filter: File: Whitelist\n");
			_dir.filterFilesWithWhitelist(m_whiteListF);
		}
		if (m_blackListFActive) {
			print(m_prefix + L"Applying filter: File: Blacklist\n");
			_dir.filterFilesWithBlacklist(m_blackListF);
		}

		print(m_prefix + L"Directory information filtered with remaining:\n\t- Directories: " +
			QString::number(_dir.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_dir.fileCount()).toStdWString() + L"\n");
		
		// #########################################################################################################################

		// Check dirs

		if (_mode == ALL) {

			unsigned long long files(0);
			unsigned long long lineCount(0);
			unsigned long long lineCountNotEmpty(0);
			unsigned long long lineCountCode(0);
			std::map<QString, unsigned long long> additionalInformation;

			runScanFiles(_dir, files, lineCount, lineCountNotEmpty, lineCountCode, additionalInformation);

			print("\n");
			setColor(Color::WHITE);
			print(m_prefix + L"Files:                   ");
			setColor(Color::GREEN);
			print(QString::number(files).toStdString() + "\n");

			setColor(Color::WHITE);
			print(m_prefix + L"Lines of text:           ");
			setColor(Color::GREEN);
			print(QString::number(lineCount).toStdString() + "\n");

			setColor(Color::WHITE);
			print(m_prefix + L"Not empty lines of text: ");
			setColor(Color::GREEN);
			print(QString::number(lineCountNotEmpty).toStdString() + "\n");

			setColor(Color::WHITE);
			print(m_prefix + L"Lines of code:           ");
			setColor(Color::GREEN);
			print(QString::number(lineCountCode).toStdString() + "\n\n");

			for (auto it : additionalInformation) {
				setColor(Color::WHITE);
				print(m_prefix + it.first.toStdWString());
				setColor(Color::GREEN);
				print(QString::number(it.second).toStdString() + "\n");
			}
			print("\n");

		}
		else if (_mode == FILESONLY) {
			runScanFilenames(L"../", _dir);
		}
		else if (_mode == DIRSONLY) {
			runScanDirectories(L"../", _dir);
		}
	}
	catch (const std::exception& _e) {
		print(m_prefix + L"Error: ");
		print(std::string(_e.what()) + "\n");
		finishPerform();
		return;
	}
	finishPerform();
}

void source::performFind(aci::aDir _dir, const std::wstring& _text) {
	try {

		// Scan

		print(m_prefix + L"Scanning: Directory\n");
		_dir.scanAll();

		print(m_prefix + L"Directory information (no filter) loaded with:\n\t- Directories: " +
			QString::number(_dir.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_dir.fileCount()).toStdWString() + L"\n");

		// #########################################################################################################################

		// Apply filter

		if (m_whiteListDActive) {
			print(m_prefix + L"Applying filter: Directory: Whitelist\n");
			_dir.filterDirectoriesWithWhitelist(m_whiteListD);
		}
		if (m_blackListDActive) {
			print(m_prefix + L"Applying filter: Directory: Blacklist\n");
			_dir.filterDirectoriesWithBlacklist(m_blackListD);
		}
		if (m_whiteListFActive) {
			print(m_prefix + L"Applying filter: File: Whitelist\n");
			_dir.filterFilesWithWhitelist(m_whiteListF);
		}
		if (m_blackListFActive) {
			print(m_prefix + L"Applying filter: File: Blacklist\n");
			_dir.filterFilesWithBlacklist(m_blackListF);
		}

		print(m_prefix + L"Directory information filtered with remaining:\n\t- Directories: " +
			QString::number(_dir.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_dir.fileCount()).toStdWString() + L"\n");

		// #########################################################################################################################

		// Check dirs

		std::list<std::pair<std::wstring, unsigned long long>> matches;
		findScanDirectory(_dir, L"..", QString::fromStdWString(_text), matches);

		size_t longestPath = 0;
		for (auto m : matches) {
			if (m.first.length() > longestPath) longestPath = m.first.length();
		}
		longestPath += 4;

		print(L"\n" + fillString(L"File", longestPath) + L"Line\n");
		for (auto m : matches) {
			print(fillString(m.first, longestPath) + std::to_wstring(m.second) + L"\n");
		}

	}
	catch (const std::exception& _e) {
		print(m_prefix + L"Error: ");
		print(std::string(_e.what()) + "\n");
		finishPerform();
		return;
	}
	finishPerform();
}

void source::runScanDirectories(const std::wstring& _pathPrefix, const aci::aDir& _dir) {
	for (auto dir : _dir.subdirectories()) {
		setColor(Color::WHITE);
		print(m_prefix);
		setColor(Color::GREEN);
		print(_pathPrefix + dir->name() + L"\n");
		runScanDirectories(_pathPrefix + dir->name() + L"/", *dir);
	}
}

void source::runScanFilenames(const std::wstring& _pathPrefix, const aci::aDir& _dir) {
	for (auto f : _dir.files()) {
		setColor(Color::WHITE);
		print(m_prefix);
		setColor(Color::GREEN);
		print(_pathPrefix + f->name() + L"\n");
	}
	for (auto dir : _dir.subdirectories()) {
		runScanFilenames(_pathPrefix + dir->name() + L"/", *dir);
	}
}

void source::runScanFiles(const aci::aDir& _dir, unsigned long long& _files, unsigned long long& _textLines, unsigned long long& _nonEmptyLines, unsigned long long& _sourceLines, std::map<QString, unsigned long long>& _additionalInformation) {
	for (auto file : _dir.files()) {
		QFile actualFile(QString::fromStdWString(file->fullPath()));
		if (actualFile.open(QIODevice::ReadOnly)) {
			_files++;

			QTextStream stream(&actualFile);

			bool isComment{ false };
			while (!stream.atEnd()) {
				QString line = stream.readLine();
				_textLines++;

				// Check if the line is an actual line of code
				if (!line.isEmpty()) {
					line.remove(" ");
					line.remove("\t");
					if (!line.isEmpty() && line != "\n") {
						_nonEmptyLines++;
						if (m_language == L"c++") runCheckSyntaxCPP(line, isComment, _sourceLines, _additionalInformation);
						else _sourceLines++;
					}
				}
			}
			actualFile.close();
		}
		else {
			setColor(Color::WHITE);
			print(m_prefix);
			setColor(Color::RED);
			print(L"Failed to open file for reading: " + file->fullPath() + L"\n");
		}
	}

	for (auto subdir : _dir.subdirectories()) {
		runScanFiles(*subdir, _files, _textLines, _nonEmptyLines, _sourceLines, _additionalInformation);
	}

}

void source::runCheckSyntaxCPP(const QString& _line, bool& _isComment, unsigned long long& _sourceLines, std::map<QString, unsigned long long>& _additionalInformation) {
	bool added{ false };

	if (_line.startsWith("//")) return;
	if (_line.contains("/*")) {
		{
			QStringList cont = _line.split("/*", QString::SkipEmptyParts);
			if (!_isComment && cont.count() > 1) {
				_sourceLines++;
				added = true;
			}
			_isComment = true;
		}
		if (_line.contains("*/")) {
			QStringList cont = _line.split("*/", QString::SkipEmptyParts);
			if (cont.count() > 1 && !added) {
				_sourceLines++;
			}
		}
		return;
	}
	if (_line.contains("*/")) {
		QStringList cont = _line.split("*/", QString::SkipEmptyParts);
		if (cont.count() > 1) {
			_sourceLines++;
		}
		if (_line.contains("/*")) {
			if (_line.indexOf("*/") > _line.indexOf("/*")) {
				_isComment = false;
			}
		}
		else _isComment = false;
		return;
	}
	if (!_isComment) {
		_sourceLines++;
		runLineHasKey(_line, "if", "if:                      ", _additionalInformation);
		runLineHasKey(_line, "switch", "switch:                  ", _additionalInformation);
		runLineHasKey(_line, "for", "for:                     ", _additionalInformation);
		runLineHasKey(_line, "while", "while:                   ", _additionalInformation);
		runLineHasKey(_line, "return", "return:                  ", _additionalInformation);
	}
}

void source::runLineHasKey(const QString& _line, const QString& _key, const QString& _keyName, std::map<QString, unsigned long long>& _informationMap) {
	if (_line.contains(_key)) {
		auto it = _informationMap.find(_keyName);
		if (it == _informationMap.end()) _informationMap.insert_or_assign(_keyName, 1);
		else _informationMap.insert_or_assign(_keyName, it->second + 1);
	}
}

void source::findScanDirectory(const aci::aDir& _dir, const std::wstring& _shortPath, const QString& _textToFind, std::list<std::pair<std::wstring, unsigned long long>>& _matches) {
	// Check files
	for (auto f : _dir.files()) {
		QFile actualFile(QString::fromStdWString(f->fullPath()));
		if (actualFile.open(QIODevice::ReadOnly)) {
			QTextStream stream(&actualFile);

			unsigned long long lineCt = 0;
			while (!stream.atEnd()) {
				QString line = stream.readLine();
				lineCt++;
				QRegExp rx(_textToFind);
				rx.setMinimal(true);
				if (line.contains(rx)) 
					_matches.push_back(std::pair<std::wstring, unsigned long long>(_shortPath + L"/" + f->name(), lineCt));
			}
			actualFile.close();
		}
		else {
			setColor(Color::WHITE);
			print(m_prefix);
			setColor(Color::RED);
			print(L"Failed to open file for reading: " + f->fullPath() + L"\n");
		}
	}

	// Check directories
	for (auto d : _dir.subdirectories()) {
		findScanDirectory(*d, _shortPath + L"/" + d->name(), _textToFind, _matches);
	}
}

void source::finishPerform() {
	setColor(Color::WHITE);
	print(m_prefix + L"Done\n");
	enableInput();
}

std::wstring source::fillString(const std::wstring& _original, size_t _length) {
	std::wstring ret(_original);
	while (ret.length() < _length) ret.append(L" ");
	return ret;
}
