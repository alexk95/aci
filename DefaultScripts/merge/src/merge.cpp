#include "merge.h"
#include <aci/aFile.h>
#include <AdditionalOperators.h>

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qtextstream.h>

#include <thread>

merge::merge(void)
	: m_searchTopDirectoryOnly(false), m_blackListDActive(false), m_whiteListDActive(false),
	m_blackListFActive(false), m_whiteListFActive(false), m_autoMerge(false), m_prefix(L"[merge] ")
{}

merge::~merge() {

}

bool merge::handle(const std::wstring& _command, const std::list<std::wstring>& _params) {
	if (_params.size() == 2) {
		if (_params.back() == L"cfg") { cmdConfiguration(); return true; }
		else if (_params.back() == L"clear") { cmdClear(); return true; }
		else if (_params.back() == L"run" || _params.back() == L"merge") { return run(false); }
		else if (_params.back() == L"dif") { return run(true); }
		else {
			setColor(255, 0, 0);
			print(m_prefix + L"Invalid arguments for \"merge\". Try \"?merge\" for help\n");
			return false;
		}
	}
	else if (_params.size() == 3) {
		if (*std::next(_params.begin()) == L"wd") {
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
				setColor(255, 0, 0);
				print(m_prefix + L"Invalid value for \"Directory whitelist active = <value>\"\n");
				return false;
			}
		}
		else if (*std::next(_params.begin()) == L"+wd") {
			for (auto itm : m_whiteListD) {
				if (itm == _params.back()) { return true; }
			}
			m_whiteListD.push_back(_params.back());
			return true;
		}
		else if (*std::next(_params.begin()) == L"-wd") {
			if (_params.back() == L"*") { m_whiteListD.clear(); return true; }
			for (auto it = m_whiteListD.begin(); it != m_whiteListD.end(); it++) {
				if (*it == _params.back()) {
					m_whiteListD.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print(m_prefix + L"Directory whitelist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (*std::next(_params.begin()) == L"bd") {
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
				setColor(255, 0, 0);
				print(m_prefix + L"Invalid value for \"Directory blacklist active = <value>\"\n");
				return false;
			}
		}
		else if (*std::next(_params.begin()) == L"+bd") {
			for (auto itm : m_blackListD) {
				if (itm == _params.back()) { return true; }
			}
			m_blackListD.push_back(_params.back());
			return true;
		}
		else if (*std::next(_params.begin()) == L"-bd") {
			if (_params.back() == L"*") { m_blackListD.clear(); return true; }
			for (auto it = m_blackListD.begin(); it != m_blackListD.end(); it++) {
				if (*it == _params.back()) {
					m_blackListD.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print(m_prefix + L"Directory blacklist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (*std::next(_params.begin()) == L"wf") {
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
				setColor(255, 0, 0);
				print(m_prefix + L"Invalid value for \"File whitelist active = <value>\"\n");
				return false;
			}
		}
		else if (*std::next(_params.begin()) == L"+wf") {
			for (auto itm : m_whiteListF) {
				if (itm == _params.back()) { return true; }
			}
			m_whiteListF.push_back(_params.back());
			return true;
		}
		else if (*std::next(_params.begin()) == L"-wf") {
			if (_params.back() == L"*") { m_whiteListF.clear(); return true; }
			for (auto it = m_whiteListF.begin(); it != m_whiteListF.end(); it++) {
				if (*it == _params.back()) {
					m_whiteListF.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print(m_prefix + L"File whitelist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (*std::next(_params.begin()) == L"bf") {
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
				setColor(255, 0, 0);
				print(m_prefix + L"Invalid value for \"File blacklist active = <value>\"\n");
				return false;
			}
		}
		else if (*std::next(_params.begin()) == L"+bf") {
			for (auto itm : m_blackListF) {
				if (itm == _params.back()) { return true; }
			}
			m_blackListF.push_back(_params.back());
			return true;
		}
		else if (*std::next(_params.begin()) == L"-bf") {
			if (_params.back() == L"*") { m_blackListF.clear(); return true; }
			for (auto it = m_blackListF.begin(); it != m_blackListF.end(); it++) {
				if (*it == _params.back()) {
					m_blackListF.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print(m_prefix + L"File blacklist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (*std::next(_params.begin()) == L"save") {
			return cmdSave(QString::fromStdWString(_params.back()));
		}
		else if (*std::next(_params.begin()) == L"load") {
			return cmdLoad(QString::fromStdWString(_params.back()));
		}
		else if (*std::next(_params.begin()) == L"d1") {
			m_pathOne = _params.back();
			return true;
		}
		else if (*std::next(_params.begin()) == L"d2") {
			m_pathTwo = _params.back();
			return true;
		}
		else if (*std::next(_params.begin()) == L"top") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") { m_searchTopDirectoryOnly = true; return true; }
			else if (opt == "false" || opt == "0") { m_searchTopDirectoryOnly = false; return true; }
			else {
				setColor(255, 0, 0);
				print(m_prefix + L"Invalid value for \"Top level only active = <value>\"\n");
				return false;
			}
		}
		else if (*std::next(_params.begin()) == L"auto") {
			QString opt = QString::fromStdWString(_params.back());
			opt = opt.toLower();
			if (opt == "true" || opt == "1") { m_autoMerge = true; return true; }
			else if (opt == "false" || opt == "0") { m_autoMerge = false; return true; }
			else {
				setColor(255, 0, 0);
				print(m_prefix + L"Invalid value for \"Auto merge active = <value>\"\n");
				return false;
			}
		}
	}

	setColor(255, 0, 0);
	print(m_prefix + L"Invalid arguments for \"" + key() + L"\"\n");
	return false;
}

// #####################################################################################################

void merge::cmdConfiguration(void) {
	showDelimiterLine();
	print(L"###  " + key() + L" configuration  ###\n");

	print(L"Directory 1:                     ");
	if (m_pathOne.empty()) { print(L"<empty>\n"); }
	else { print(m_pathOne + L"\n"); }

	print("Directory 2:                     ");
	if (m_pathTwo.empty()) { print("<empty>\n"); }
	else { print(m_pathTwo + L"\n"); }

	print("Search only top level directory: ");
	print(m_searchTopDirectoryOnly);
	setColor(255, 255, 255);
	print("\n");

	print("Auto merge:                      ");
	print(m_autoMerge);
	setColor(255, 255, 255);
	print("\n");

	print("Whilelist (directories) active:  ");
	print(m_whiteListDActive);
	setColor(255, 255, 255);
	print("\n");

	print("Whitelist (directories):\n");
	if (m_whiteListD.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_whiteListD) { print(L"\t" + itm + L"\n"); }
	}

	print("Blacklist (directories) active:  ");
	print(m_blackListDActive);
	setColor(255, 255, 255);
	print("\n");

	print("Blacklist (directories):\n");
	if (m_blackListD.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_blackListD) { print(L"\t" + itm + L"\n"); }
	}

	print("Whilelist (files) active:        ");
	print(m_whiteListFActive);
	setColor(255, 255, 255);
	print("\n");

	print("Whitelist (files):\n");
	if (m_whiteListF.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_whiteListF) { print(L"\t" + itm + L"\n"); }
	}

	print("Blacklist (files) active:        ");
	print(m_blackListFActive);
	setColor(255, 255, 255);
	print("\n");

	print("Blacklist (files):\n");
	if (m_blackListF.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_blackListF) { print(L"\t" + itm + L"\n"); }
	}
}

void merge::cmdClear(void) {
	m_blackListDActive = false;
	m_whiteListDActive = false;
	m_blackListFActive = false;
	m_whiteListFActive = false;

	m_blackListD.clear();
	m_whiteListD.clear();
	m_blackListF.clear();
	m_whiteListF.clear();

	m_pathOne.clear();
	m_pathTwo.clear();

	m_searchTopDirectoryOnly = false;
	m_autoMerge = false;

	print("[merge] Configuration cleared\n");
}

bool merge::cmdLoad(const QString& _filename) {
	if (_filename == "?") { showSavedConfigs(); return true; }

	std::wstring data;
	if (!readDataFile(data, ("Configurations/" + _filename + ".json").toStdWString(), true)) {
		return false;
	}

	print("[merge] [parse]: Starting parser\n");
	QStringList lst;
	QByteArray byteData;
	byteData.append(QString::fromStdWString(data));

	QJsonDocument doc = QJsonDocument::fromJson(byteData);
	if (!doc.isObject()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Invalid document format\n");
		return false;
	}

	// Check member
	QJsonObject obj = doc.object();
	if (!obj.contains("DirectoryOne")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryOne\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryTwo")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryTwo\" is missing\n");
		return false;
	}
	if (!obj.contains("SearchTopOnly")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"SearchTopOnly\" is missing\n");
		return false;
	}
	if (!obj.contains("AutoMerge")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"AutoMerge\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryWhitelistActive")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryWhitelistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryWhitelist")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryWhitelist\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryBlacklistActive")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryBlacklistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("DirectoryBlacklist")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryBlacklist\" is missing\n");
		return false;
	}
	if (!obj.contains("FileWhitelistActive")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileWhitelistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("FileWhitelist")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileWhitelist\" is missing\n");
		return false;
	}
	if (!obj.contains("FileBlacklistActive")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileBlacklistActive\" is missing\n");
		return false;
	}
	if (!obj.contains("FileBlacklist")) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileBlacklist\" is missing\n");
		return false;
	}

	// Check data types
	if (!obj["DirectoryOne"].isString()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryOne\" is not a string\n");
		return false;
	}
	if (!obj["DirectoryTwo"].isString()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryTwo\" is not a string\n");
		return false;
	}
	if (!obj["SearchTopOnly"].isBool()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"SearchTopOnly\" is not a boolean\n");
		return false;
	}
	if (!obj["AutoMerge"].isBool()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"AutoMerge\" is not a boolean\n");
		return false;
	}
	if (!obj["DirectoryWhitelistActive"].isBool()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryWhitelistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["DirectoryWhitelist"].isArray()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryWhitelist\" is not a array\n");
		return false;
	}
	if (!obj["DirectoryBlacklistActive"].isBool()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryBlacklistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["DirectoryBlacklist"].isArray()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"DirectoryBlacklist\" is not a array\n");
		return false;
	}
	if (!obj["FileWhitelistActive"].isBool()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileWhitelistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["FileWhitelist"].isArray()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileWhitelist\" is not a array\n");
		return false;
	}
	if (!obj["FileBlacklistActive"].isBool()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileBlacklistActive\" is not a boolean\n");
		return false;
	}
	if (!obj["FileBlacklist"].isArray()) {
		print("[merge] [parse]: ");
		setColor(255, 0, 0);
		print("Member \"FileBlacklist\" is not a array\n");
		return false;
	}

	// Collect data
	m_pathOne = obj["DirectoryOne"].toString().toStdWString();
	m_pathTwo = obj["DirectoryTwo"].toString().toStdWString();
	m_searchTopDirectoryOnly = obj["SearchTopOnly"].toBool();
	m_autoMerge = obj["AutoMerge"].toBool();

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
			assert(0); // "Invalid data type @merge @load"
		}
	}

	QJsonArray blacklistD = obj["DirectoryBlacklist"].toArray();
	m_blackListD.clear();
	for (int i{ 0 }; i < blacklistD.count(); i++) {
		if (blacklistD[i].isString()) {
			m_blackListD.push_back(blacklistD[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @merge @load"
		}
	}

	QJsonArray whitelistF = obj["FileWhitelist"].toArray();
	m_whiteListF.clear();
	for (int i{ 0 }; i < whitelistF.count(); i++) {
		if (whitelistF[i].isString()) {
			m_whiteListF.push_back(whitelistF[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @merge @load"
		}
	}

	QJsonArray blacklistF = obj["FileBlacklist"].toArray();
	m_blackListF.clear();
	for (int i{ 0 }; i < blacklistF.count(); i++) {
		if (blacklistF[i].isString()) {
			m_blackListF.push_back(blacklistF[i].toString().toStdWString());
		}
		else {
			assert(0); // "Invalid data type @merge @load"
		}
	}

	print("[merge] [parse]: Import successfull\n");

	return true;
}

bool merge::cmdSave(const QString& _filename) {
	if (_filename == "?") { showSavedConfigs(); return true; }

	// Collect data
	QJsonObject obj;
	obj["DirectoryOne"] = QString::fromStdWString(m_pathOne);
	obj["DirectoryTwo"] = QString::fromStdWString(m_pathTwo);
	obj["SearchTopOnly"] = m_searchTopDirectoryOnly;
	obj["AutoMerge"] = m_autoMerge;

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
		print("[merge]: Export successfull\n");
		return true;
	}
	return false;
}

void merge::showSavedConfigs(void) {
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

// Merger

bool merge::run(bool _difOnly) {

	if (m_pathOne.empty()) {
		setColor(255, 0, 0);
		print(m_prefix + L"Diretory one is empty\n");
		return false;
	}
	if (m_pathTwo.empty()) {
		setColor(255, 0, 0);
		print(m_prefix + L"Diretory one is empty\n");
		return false;
	}

	// Check the directories
	aci::aDir d1(L"", m_pathOne);
	aci::aDir d2(L"", m_pathTwo);

	if (!d1.exists()) {
		setColor(255, 0, 0);
		print(m_prefix + L"The directory \"" + m_pathOne + L"\" does not exist\n");
		return false;
	}

	if (!d2.exists()) {
		setColor(255, 0, 0);
		print(m_prefix + L"The directory \"" + m_pathTwo + L"\" does not exist\n");
		return false;
	}

	disableInput();

	std::thread t1(&merge::performRun, this, _difOnly, d1, d2);
	t1.detach();
	return true;
}

// #####################################################################################################

// Slots

void merge::finishRun(bool _difOnly) {
	queueColor(255, 255, 255);
	if (_difOnly) { queuePrint(m_prefix + L"[dif]: Done\n"); }
	else { queuePrint(m_prefix + L"[merge]: Done\n"); }
	queueEnableInput();
}

// #####################################################################################################

// Protected functions

void merge::showCommandInfo(void) {
	print(m_prefix + L"? Merge is used to update a project that is located in two different locations\n");
	print(m_prefix + L"? Merge will compare missing folders and files, and replace outdated files\n");
	print(m_prefix + L"? with the newest version from the other directory\n\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tcfg               ");
	setColor(255, 255, 255);
	print(m_prefix + L"Show the current configuration\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\ttop <active>      ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will activate/disable the top level only option ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tauto <active>     ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will activate/disable the auto merge option ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tload ?            ");
	setColor(255, 255, 255);
	print(m_prefix + L"Show all saved configurations\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tsave ?            ");
	setColor(255, 255, 255);
	print(m_prefix + L"Show all saved configurations\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\trun ?             ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will start the merge with the current configuration\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tmerge ?           ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will start the merge with the current configuration\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tdif ?             ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will start the dif with the current configuration\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\td1 <path>         ");
	setColor(255, 255, 255);
	print(m_prefix + L"The the directory 1\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\td2 <path>         ");
	setColor(255, 255, 255);
	print(m_prefix + L"The the directory 2\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tload <filename>   ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will load the configuration from the file specified\n"
		"\t                       (");
	setColor(255, 150, 50);
	print(m_prefix + L"\"../ScriptData/merge/Configurations");
	setColor(0, 255, 0);
	print(m_prefix + L"<filename>");
	setColor(255, 150, 50);
	print(m_prefix + L".json\")\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tsave <filename>   ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will save the configuration to the file specified\n"
		"\t                       (");
	setColor(255, 150, 50);
	print(m_prefix + L"\"../ScriptData/merge/Configurations");
	setColor(0, 255, 0);
	print(m_prefix + L"<filename>");
	setColor(255, 150, 50);
	print(m_prefix + L".json\")\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\twd <active>       ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will activate/disable the whitelist for directories ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\t+wd <filter>      ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will add the provided filter to the directories whitelist\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\t-wd <filter>      ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will remove the provided filter from the directories whitelist\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\t-wd *             ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will clear the directories whitelist\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\tbd <active>       ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will activate/disable the blacklist for directories ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\t+bd <filter>      ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will add the provided filter to the directories blacklist\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\t-bd <filter>      ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will remove the provided filter from directories the blacklist\n");

	setColor(255, 150, 50);
	print(m_prefix + L"\t-bd *             ");
	setColor(255, 255, 255);
	print(m_prefix + L"Will clear the directories blacklist\n");
}

// #####################################################################################################

// Private functions

void merge::performRun(bool _difOnly, aci::aDir _d1, aci::aDir _d2) {
	std::wstring prefix;
	if (_difOnly) { prefix = m_prefix + L"[dif]: "; }
	else { prefix = m_prefix + L"[merge]: "; }

	try {

		// Scan

		queuePrint(prefix + L"Scanning: Directory 1\n");
		_d1.scanAll();
		queuePrint(prefix + L"Scanning: Directory 2\n");
		_d2.scanAll();

		queuePrint(prefix + L"Directory 1 information (no filter) loaded with:\n\t- Directories: " +
			QString::number(_d1.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_d1.fileCount()).toStdWString() + L"\n");

		queuePrint(prefix + L"Directory 2 information (no filter) loaded with:\n\t- Directories: " +
			QString::number(_d2.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_d2.fileCount()).toStdWString() + L"\n");

		// #########################################################################################################################

		// Apply filter

		if (m_whiteListDActive) {
			queuePrint(prefix + L"Applying filter: Directory: Whitelist: Directory 1\n");
			_d1.filterDirectoriesWithWhitelist(m_whiteListD);
			queuePrint(prefix + L"Applying filter: Directory: Whitelist: Directory 2\n");
			_d2.filterDirectoriesWithWhitelist(m_whiteListD);
		}
		if (m_blackListDActive) {
			queuePrint(prefix + L"Applying filter: Directory: Blacklist: Directory 1\n");
			_d1.filterDirectoriesWithBlacklist(m_blackListD);
			queuePrint(prefix + L"Applying filter: Directory: Blacklist: Directory 2\n");
			_d2.filterDirectoriesWithBlacklist(m_blackListD);
		}
		if (m_whiteListFActive) {
			queuePrint(prefix + L"Applying filter: File: Whitelist: Directory 1\n");
			_d1.filterFilesWithWhitelist(m_whiteListF);
			queuePrint(prefix + L"Applying filter: File: Whitelist: Directory 2\n");
			_d2.filterFilesWithWhitelist(m_whiteListF);
		}
		if (m_blackListFActive) {
			queuePrint(prefix + L"Applying filter: File: Blacklist: Directory 1\n");
			_d1.filterFilesWithBlacklist(m_blackListF);
			queuePrint(prefix + L"Applying filter: File: Blacklist: Directory 2\n");
			_d2.filterFilesWithBlacklist(m_blackListF);
		}

		queuePrint(prefix + L"Directory 1 information filtered with remaining:\n\t- Directories: " +
			QString::number(_d1.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_d1.fileCount()).toStdWString() + L"\n");
		
		queuePrint(prefix + L"Directory 2 information filtered with remaining:\n\t- Directories: " +
			QString::number(_d2.subDirCount()).toStdWString() + L"\n\t- Files: " + QString::number(_d2.fileCount()).toStdWString() + L"\n");

		// #########################################################################################################################

		// Check dirs

		queuePrint(prefix + L"Check directories\n");
		if (checkDirectories(_difOnly, _d1, _d2, prefix) && (_difOnly || !m_autoMerge)) {
			queueColor(255, 150, 50);
			queuePrint(prefix + L"! > Execution stopped. Continue manually\n");
			finishRun(_difOnly);
			return;
		}
		queueColor(255, 255, 255);
		queuePrint(prefix + L"Check directories ");
		queueColor(0, 255, 0);
		queuePrint("NO DIF\n");
		queueColor(255, 255, 255);

		// #########################################################################################################################

		// Check files existance

		queuePrint(prefix + L"Check files (existance)\n");
		if (checkFiles(_difOnly, _d1, _d2, prefix) && (_difOnly || !m_autoMerge)) {
			queueColor(255, 150, 50);
			queuePrint(prefix + L"! > Execution stopped. Continue manually\n");
			finishRun(_difOnly);
			return;
		}
		queueColor(255, 255, 255);
		queuePrint(prefix + L"Check files (existance) ");
		queueColor(0, 255, 0);
		queuePrint("NO DIF\n");
		queueColor(255, 255, 255);

		// #########################################################################################################################

		// Check file content

		queuePrint(prefix + L"Check files (content)\n");
		if (checkFileContent(_difOnly, _d1, _d2, prefix)) {
			queueColor(255, 255, 255);
			queuePrint(prefix + L"Check files (content) ");
			queueColor(0, 255, 0);
			queuePrint("DONE\n");
			queueColor(255, 255, 255);
		}
		else {
			queueColor(255, 255, 255);
			queuePrint(prefix + L"Check files (content) ");
			queueColor(0, 255, 0);
			queuePrint("NO DIF\n");
			queueColor(255, 255, 255);
		}
	}
	catch (const std::exception& _e) {
		queuePrint(prefix + L"Check directories\n");
		finishRun(_difOnly);
		
	}
	finishRun(_difOnly);
}

bool merge::checkDirectories(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix) {

	bool ret{ false };

	std::list<std::wstring> sub1 = _d1.subdirectoryNameList();
	std::list<std::wstring> sub2 = _d2.subdirectoryNameList();

	// ##### 1 > 2

	queueColor(255, 150, 50);
	queuePrint(_logPrefix + L"##############################################################\n");
	queueColor(255, 255, 255);
	queuePrint(_logPrefix + L"     From: " + _d1.fullPath() + L"\n" +
		_logPrefix + L"     To:   " + _d2.fullPath() + L"\n\n");

	for (auto s1 : sub1) {
		bool found{ false };
		for (auto s2 : sub2) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(255, 0, 0);
				queuePrint(_logPrefix + L"Missing: " + s1 + L"\n");
			}
			else {
				queueColor(255, 150, 50);
				queuePrint(_logPrefix + L"Creating: " + s1 + L"\n");
				QDir().mkdir(QString::fromStdWString(_d2.fullPath() + L"/" + s1));
			}
			ret = true;
		}
	}

	// ##### 2 > 1

	queueColor(255, 150, 50);
	queuePrint(_logPrefix + L"##############################################################\n");
	queueColor(255, 255, 255);
	queuePrint(_logPrefix + L"     From: " + _d2.fullPath() + L"\n" +
		_logPrefix + L"     To:   " + _d1.fullPath() + L"\n\n");

	for (auto s2 : sub2) {
		bool found{ false };
		for (auto s1 : sub1) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(255, 0, 0);
				queuePrint(_logPrefix + L"Missing: " + s2 + L"\n");
			}
			else {
				queueColor(255, 150, 50);
				queuePrint(_logPrefix + L"Creating: " + s2 + L"\n");
				if (!QDir().mkdir(QString::fromStdWString(_d1.fullPath() + L"/" + s2))) {
					queueColor(255, 0, 0);
					queuePrint(_logPrefix + L"Failed to create directory (Auto merge disabled now)\n");
					m_autoMerge = false;
					return true;
				}
			}
			ret = true;
		}
	}

	return ret;
}

bool merge::checkFiles(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix) {

	bool ret{ false };

	std::list<std::wstring> files1 = _d1.fileNameList();
	std::list<std::wstring> files2 = _d2.fileNameList();

	// ##### 1 > 2

	queueColor(255, 150, 50);
	queuePrint(_logPrefix + L"##############################################################\n");
	queueColor(255, 255, 255);
	queuePrint(_logPrefix + L"     From: " + _d1.fullPath() + L"\n" +
		_logPrefix + L"     To:   " + _d2.fullPath() + L"\n\n");

	for (auto s1 : files1) {
		bool found{ false };
		for (auto s2 : files2) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(255, 0, 0);
				queuePrint(_logPrefix + L"Missing: " + s1 + L"\n");
			}
			else {
				queueColor(255, 150, 50);
				queuePrint(_logPrefix + L"Creating: " + s1 + L"\n");

				QFile file(QString::fromStdWString(_d1.fullPath() + L"/" + s1));
				if (!file.copy(QString::fromStdWString(_d2.fullPath() + L"/" + s1))) {
					queueColor(255, 0, 0);
					queuePrint(_logPrefix + L"Failed to copy file (Auto merge disabled now)\n");
					m_autoMerge = false;
					return true;
				}
			}
			ret = true;
		}
	}

	// ##### 2 > 1

	queueColor(255, 150, 50);
	queuePrint(_logPrefix + L"##############################################################\n");
	queueColor(255, 255, 255);
	queuePrint(_logPrefix + L"     From: " + _d2.fullPath() + L"\n" +
		_logPrefix + L"     To:   " + _d1.fullPath() + L"\n\n");

	for (auto s2 : files2) {
		bool found{ false };
		for (auto s1 : files1) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(255, 0, 0);
				queuePrint(_logPrefix + L"Missing: " + s2 + L"\n");
			}
			else {
				queueColor(255, 150, 50);
				queuePrint(_logPrefix + L"Creating: " + s2 + L"\n");

				QFile file(QString::fromStdWString(_d2.fullPath() + L"/" + s2));
				if (!file.copy(QString::fromStdWString(_d1.fullPath() + L"/" + s2))) {
					queueColor(255, 0, 0);
					queuePrint(_logPrefix + L"Failed to copy file (Auto merge disabled now)\n");
					m_autoMerge = false;
					return true;
				}
			}
			ret = true;
		}
	}

	return ret;
}

bool merge::checkFileContent(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix) {
	bool ret{ false };

	std::list<std::wstring> files1 = _d1.fileNameList();
	std::list<std::wstring> files2 = _d2.fileNameList();

	// ##### 1 > 2

	queueColor(255, 150, 50);
	queuePrint(_logPrefix + L"##############################################################\n");
	queueColor(255, 255, 255);
	queuePrint(_logPrefix + _d1.fullPath() + L"\n");
	queuePrint(_logPrefix + L"                         " + _d2.fullPath() + L"\n");

	for (auto f1 : files1) {
		// File 1
		QFile file1(QString::fromStdWString(_d1.fullPath() + L"/" + f1));
		if (!file1.exists()) {
			queueColor(255, 0, 0);
			queuePrint(_logPrefix + L"File does not exist (" + _d1.fullPath() + L"/" + f1 + L")\n");
			return true;
		}
		if (!file1.open(QIODevice::ReadOnly)) {
			queueColor(255, 0, 0);
			queuePrint(_logPrefix + L"Failed to open file for reading (" + _d1.fullPath() + L"/" + f1 + L")\n");
			return true;
		}

		QByteArray data1 = file1.readAll();
		file1.close();

		// File 2
		QFile file2(QString::fromStdWString(_d2.fullPath() + L"/" + f1));
		if (!file2.exists()) {
			queueColor(255, 0, 0);
			queuePrint(_logPrefix + L"File does not exist (" + _d2.fullPath() + L"/" + f1 + L")\n");
			return true;
		}
		if (!file2.open(QIODevice::ReadOnly)) {
			queueColor(255, 0, 0);
			queuePrint(_logPrefix + L"Failed to open file for reading (" + _d2.fullPath() + L"/" + f1 + L")\n");
			return true;
		}

		QByteArray data2 = file2.readAll();
		file2.close();

		// Compare
		if (data1 != data2) {
			QFileInfo info1(file1);
			QFileInfo info2(file2);
			if (info1.lastModified() > info2.lastModified()) {
				if (_difOnly) {
					queueColor(0, 255, 0);
					queuePrint(_logPrefix + f1 + L" [NEWER]\n");
				}
				else {
					// Delete old file
					if (!file2.remove()) {
						queueColor(255, 0, 0);
						queuePrint(_logPrefix + L"Failed to delete outdated file (" + _d2.fullPath() + L"/" + f1 + L")\n");
						return true;
					}

					// Copy newer file
					if (!file1.copy(QString::fromStdWString(_d2.fullPath() + L"/" + f1))) {
						queueColor(255, 0, 0);
						queuePrint(_logPrefix + L"Failed to copy file (" + _d1.fullPath() + L"/" + f1 + L"  ->  " + _d2.fullPath() + L"/" + f1 + L")\n");
						return true;
					}

					queueColor(255, 150, 50);
					queuePrint(_logPrefix + L"                         Updated: " + _d2.fullPath() + L"/" + f1 + L"\n");
				}
			}
			else if (info1.lastModified() < info2.lastModified()) {
				if (_difOnly) {
					queueColor(0, 255, 0);
					queuePrint(_logPrefix + L"                         " + f1 + L" [NEWER]\n");
				}
				else {
					// Delete old file
					if (!file1.remove()) {
						queueColor(255, 0, 0);
						queuePrint(_logPrefix + L"Failed to delete outdated file (" + _d1.fullPath() + L"/" + f1 + L")\n");
						return true;
					}

					// Copy newer file
					if (!file2.copy(QString::fromStdWString(_d1.fullPath() + L"/" + f1))) {
						queueColor(255, 0, 0);
						queuePrint(_logPrefix + L"Failed to copy file (" + _d2.fullPath() + L"/" + f1 + L"  ->  " + _d1.fullPath() + L"/" + f1 + L")\n");
						return true;
					}

					queueColor(255, 150, 50);
					queuePrint(_logPrefix + L"Updated: " + _d1.fullPath() + L"/" + f1 + L"\n");
				}
			}
			else {
				queueColor(255, 150, 50);
				queuePrint(_logPrefix + L"Can not merge \"" + f1 + L"\"\n");
				queuePrint(_logPrefix + L"       ^-- Files differ with same last modified value\n");
				if (!_difOnly) { return true; }
			}
			ret = true;
		}

	}

	return ret;
}
