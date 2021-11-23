#include <sMerge.h>
#include <AppBase.h>
#include <aFile.h>
#include <AdditionalOperators.h>

#include <akCore/aAssert.h>

#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qdir.h>
#include <qfileinfo.h>

#include <thread>

sMerge::sMerge(AppBase * _app) 
	: m_app(_app), m_searchTopDirectoryOnly(false), m_blackListDActive(false), m_whiteListDActive(false),
	m_blackListFActive(false), m_whiteListFActive(false), m_autoMerge(false)
{}

sMerge::~sMerge() {

}

bool sMerge::handle(const QString& _command, const QStringList& _params) {
	if (_params.length() == 2) {
		if (_params.back() == "cfg") { cmdConfiguration(); return true; }
		else if (_params.back() == "clear") { cmdClear(); return true; }
		else if (_params.back() == "run" || _params.back() == "merge") { return run(false); }
		else if (_params.back() == "dif") { return run(true); }
		else {
			setColor(255, 0, 0);
			print("Invalid arguments for \"merge\". Try \"merge ?\" for help\n");
			return false;
		}
	}
	else if (_params.length() == 3) {
		if (_params[1] == "wd") {
			QString opt = _params.back();
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
				print("Invalid value for \"Directory whitelist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == "+wd") {
			for (auto itm : m_whiteListD) {
				if (itm == _params.back()) { return true; }
			}
			m_whiteListD.push_back(_params.back());
			return true;
		}
		else if (_params[1] == "-wd") {
			if (_params.back() == "*") { m_whiteListD.clear(); return true; }
			for (auto it = m_whiteListD.begin(); it != m_whiteListD.end(); it++) {
				if (*it == _params.back()) {
					m_whiteListD.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print("Directory whitelist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == "bd") {
			QString opt = _params.back();
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
				print("Invalid value for \"Directory blacklist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == "+bd") {
			for (auto itm : m_blackListD) {
				if (itm == _params.back()) { return true; }
			}
			m_blackListD.push_back(_params.back());
			return true;
		}
		else if (_params[1] == "-bd") {
			if (_params.back() == "*") { m_blackListD.clear(); return true; }
			for (auto it = m_blackListD.begin(); it != m_blackListD.end(); it++) {
				if (*it == _params.back()) {
					m_blackListD.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print("Directory blacklist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if(_params[1] == "wf") {
			QString opt = _params.back();
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
				print("Invalid value for \"File whitelist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == "+wf") {
			for (auto itm : m_whiteListF) {
				if (itm == _params.back()) { return true; }
			}
			m_whiteListF.push_back(_params.back());
			return true;
		}
		else if (_params[1] == "-wf") {
			if (_params.back() == "*") { m_whiteListF.clear(); return true; }
			for (auto it = m_whiteListF.begin(); it != m_whiteListF.end(); it++) {
				if (*it == _params.back()) {
					m_whiteListF.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print("File whitelist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == "bf") {
			QString opt = _params.back();
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
				print("Invalid value for \"File blacklist active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == "+bf") {
			for (auto itm : m_blackListF) {
				if (itm == _params.back()) { return true; }
			}
			m_blackListF.push_back(_params.back());
			return true;
		}
		else if (_params[1] == "-bf") {
			if (_params.back() == "*") { m_blackListF.clear(); return true; }
			for (auto it = m_blackListF.begin(); it != m_blackListF.end(); it++) {
				if (*it == _params.back()) {
					m_blackListF.erase(it);
					return true;
				}
			}
			setColor(255, 0, 0);
			print("File blacklist entry \"");
			setColor(255, 150, 50);
			print(_params.back());
			setColor(255, 0, 0);
			print("\" not found\n");
			return false;
		}
		else if (_params[1] == "save") {
			return cmdSave(_params.back());
		}
		else if (_params[1] == "load") {
			return cmdLoad(_params.back());
		}
		else if (_params[1] == "d1") {
			m_pathOne = _params.back();
			return true;
		}
		else if (_params[1] == "d2") {
			m_pathTwo = _params.back();
			return true;
		}
		else if (_params[1] == "top") {
			QString opt = _params.back();
			opt = opt.toLower();
			if (opt == "true" || opt == "1") { m_searchTopDirectoryOnly = true; return true; }
			else if (opt == "false" || opt == "0") { m_searchTopDirectoryOnly = false; return true; }
			else {
				setColor(255, 0, 0);
				print("Invalid value for \"Top level only active = <value>\"\n");
				return false;
			}
		}
		else if (_params[1] == "auto") {
			QString opt = _params.back();
			opt = opt.toLower();
			if (opt == "true" || opt == "1") { m_autoMerge = true; return true; }
			else if (opt == "false" || opt == "0") { m_autoMerge = false; return true; }
			else {
				setColor(255, 0, 0);
				print("Invalid value for \"Auto merge active = <value>\"\n");
				return false;
			}
		}
	}

	setColor(255, 0, 0);
	print("Invalid arguments for \"" + key() + "\"\n");
	return false;
}

// #####################################################################################################

void sMerge::cmdConfiguration(void) {
	showDelimiterLine();
	print("###  " + key() + " configuration  ###\n");
	
	print("Directory 1:                     ");
	if (m_pathOne.isEmpty()) { print("<empty>\n"); }
	else { print(m_pathOne + "\n"); }
	
	print("Directory 2:                     ");
	if (m_pathTwo.isEmpty()) { print("<empty>\n"); }
	else { print(m_pathTwo + "\n"); }

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
		for (auto itm : m_whiteListD) { print("\t" + itm + "\n"); }
	}
	
	print("Blacklist (directories) active:  ");
	print(m_blackListDActive);
	setColor(255, 255, 255);
	print("\n");

	print("Blacklist (directories):\n");
	if (m_blackListD.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_blackListD) { print("\t" + itm + "\n"); }
	}

	print("Whilelist (files) active:        ");
	print(m_whiteListFActive);
	setColor(255, 255, 255);
	print("\n");

	print("Whitelist (files):\n");
	if (m_whiteListF.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_whiteListF) { print("\t" + itm + "\n"); }
	}

	print("Blacklist (files) active:        ");
	print(m_blackListFActive);
	setColor(255, 255, 255);
	print("\n");

	print("Blacklist (files):\n");
	if (m_blackListF.empty()) { print("\t<empty>\n"); }
	else {
		for (auto itm : m_blackListF) { print("\t" + itm + "\n"); }
	}
}

void sMerge::cmdClear(void) {
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

bool sMerge::cmdLoad(const QString& _filename) {
	if (_filename == "?") { showSavedConfigs(); return true; }

	QByteArray data;
	if (!readDataFile(data, "Configurations/" + _filename + ".json", true)) {
		return false;
	}

	print("[merge] [parse]: Starting parser\n");
	QJsonDocument doc = QJsonDocument::fromJson(data);
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
	m_pathOne = obj["DirectoryOne"].toString();
	m_pathTwo = obj["DirectoryTwo"].toString();
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
			m_whiteListD.push_back(whitelistD[i].toString());
		}
		else {
			aAssert(0, "Invalid data type @merge @load");
		}
	}

	QJsonArray blacklistD = obj["DirectoryBlacklist"].toArray();
	m_blackListD.clear();
	for (int i{ 0 }; i < blacklistD.count(); i++) {
		if (blacklistD[i].isString()) {
			m_blackListD.push_back(blacklistD[i].toString());
		}
	}

	QJsonArray whitelistF = obj["FileWhitelist"].toArray();
	m_whiteListF.clear();
	for (int i{ 0 }; i < whitelistF.count(); i++) {
		if (whitelistF[i].isString()) {
			m_whiteListF.push_back(whitelistF[i].toString());
		}
		else {
			aAssert(0, "Invalid data type @merge @load");
		}
	}

	QJsonArray blacklistF = obj["FileBlacklist"].toArray();
	m_blackListF.clear();
	for (int i{ 0 }; i < blacklistF.count(); i++) {
		if (blacklistF[i].isString()) {
			m_blackListF.push_back(blacklistF[i].toString());
		}
	}

	print("[merge] [parse]: Import successfull\n");

	return true;
}

bool sMerge::cmdSave(const QString& _filename) {
	if (_filename == "?") { showSavedConfigs(); return true; }

	// Collect data
	QJsonObject obj;
	obj["DirectoryOne"] = m_pathOne;
	obj["DirectoryTwo"] = m_pathTwo;
	obj["SearchTopOnly"] = m_searchTopDirectoryOnly;
	obj["AutoMerge"] = m_autoMerge;
	
	obj["DirectoryWhitelistActive"] = m_whiteListDActive;
	QJsonArray whitelistD;
	for (QString itm : m_whiteListD) { whitelistD.push_back(itm); }
	obj["DirectoryWhitelist"] = whitelistD;

	obj["DirectoryBlacklistActive"] = m_blackListDActive;
	QJsonArray blacklistD;
	for (QString itm : m_blackListD) { blacklistD.push_back(itm); }
	obj["DirectoryBlacklist"] = blacklistD;

	obj["FileWhitelistActive"] = m_whiteListFActive;
	QJsonArray whitelistF;
	for (QString itm : m_whiteListF) { whitelistF.push_back(itm); }
	obj["FileWhitelist"] = whitelistF;

	obj["FileBlacklistActive"] = m_blackListFActive;
	QJsonArray blacklistF;
	for (QString itm : m_blackListF) { blacklistF.push_back(itm); }
	obj["FileBlacklist"] = blacklistF;

	// Convert data
	QJsonDocument doc(obj);
	
	// Write data
	return writeDataFile(doc.toJson(), "Configurations/" + _filename + ".json", true);
}

void sMerge::showSavedConfigs(void) {
	for (auto entry : filesInDirectory("Configurations")) {
		print("\t" + isolateFilename(entry) + "\n");
	}
}

// #####################################################################################################

// Merger

bool sMerge::run(bool _difOnly) {

	if (m_pathOne.isEmpty()) {
		setColor(255, 0, 0);
		print("Diretory one is empty\n");
		return false;
	}
	if (m_pathTwo.isEmpty()) {
		setColor(255, 0, 0);
		print("Diretory one is empty\n");
		return false;
	}

	// Check the directories
	aci::aDir d1("", m_pathOne);
	aci::aDir d2("", m_pathTwo);

	if (!d1.exists()) {
		setColor(255, 0, 0);
		print("The directory \"" + m_pathOne + "\" does not exist\n");
		return false;
	}

	if (!d2.exists()) {
		setColor(255, 0, 0);
		print("The directory \"" + m_pathTwo + "\" does not exist\n");
		return false;
	}

	m_app->setInputEnabled(false);

	std::thread t1(&sMerge::performRun, this, _difOnly, d1, d2);
	t1.detach();
	return true;
}

// #####################################################################################################

// Slots

void sMerge::slotFinishRun(bool _difOnly) {
	setColor(255, 255, 255);
	if (_difOnly) { print("[merge] [dif]: Done\n"); }
	else { print("[merge] [merge]: Done\n"); }
	m_app->setInputEnabled(true);
}

// #####################################################################################################

// Protected functions

void sMerge::showCommandInfo(void) {
	print("? Merge is used to update a project that is located in two different locations\n");
	print("? Merge will compare missing folders and files, and replace outdated files\n? with the newest version from the other directory\n\n");
	
	setColor(255, 150, 50);
	print("\tcfg               ");
	setColor(255, 255, 255);
	print("Show the current configuration\n");

	setColor(255, 150, 50);
	print("\ttop <active>      ");
	setColor(255, 255, 255);
	print("Will activate/disable the top level only option ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print("\tauto <active>     ");
	setColor(255, 255, 255);
	print("Will activate/disable the auto merge option ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print("\tload ?            ");
	setColor(255, 255, 255);
	print("Show all saved configurations\n");

	setColor(255, 150, 50);
	print("\tsave ?            ");
	setColor(255, 255, 255);
	print("Show all saved configurations\n");

	setColor(255, 150, 50);
	print("\trun ?             ");
	setColor(255, 255, 255);
	print("Will start the merge with the current configuration\n");

	setColor(255, 150, 50);
	print("\tmerge ?           ");
	setColor(255, 255, 255);
	print("Will start the merge with the current configuration\n");

	setColor(255, 150, 50);
	print("\tdif ?             ");
	setColor(255, 255, 255);
	print("Will start the dif with the current configuration\n");

	setColor(255, 150, 50);
	print("\td1 <path>         ");
	setColor(255, 255, 255);
	print("The the directory 1\n");

	setColor(255, 150, 50);
	print("\td2 <path>         ");
	setColor(255, 255, 255);
	print("The the directory 2\n");

	setColor(255, 150, 50);
	print("\tload <filename>   ");
	setColor(255, 255, 255);
	print("Will load the configuration from the file specified\n"
		  "\t                       (");
	setColor(255, 150, 50);
	print("\"../ScriptData/merge/Configurations");
	setColor(0, 255, 0);
	print("<filename>");
	setColor(255, 150, 50);
	print(".json\")\n");

	setColor(255, 150, 50);
	print("\tsave <filename>   ");
	setColor(255, 255, 255);
	print("Will save the configuration to the file specified\n"
		  "\t                       (");
	setColor(255, 150, 50);
	print("\"../ScriptData/merge/Configurations");
	setColor(0, 255, 0);
	print("<filename>");
	setColor(255, 150, 50);
	print(".json\")\n");

	setColor(255, 150, 50);
	print("\twd <active>       ");
	setColor(255, 255, 255);
	print("Will activate/disable the whitelist for directories ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print("\t+wd <filter>      ");
	setColor(255, 255, 255);
	print("Will add the provided filter to the directories whitelist\n");

	setColor(255, 150, 50);
	print("\t-wd <filter>      ");
	setColor(255, 255, 255);
	print("Will remove the provided filter from the directories whitelist\n");

	setColor(255, 150, 50);
	print("\t-wd *             ");
	setColor(255, 255, 255);
	print("Will clear the directories whitelist\n");

	setColor(255, 150, 50);
	print("\tbd <active>       ");
	setColor(255, 255, 255);
	print("Will activate/disable the blacklist for directories ( true / false / 0 / 1 )\n");

	setColor(255, 150, 50);
	print("\t+bd <filter>      ");
	setColor(255, 255, 255);
	print("Will add the provided filter to the directories blacklist\n");

	setColor(255, 150, 50);
	print("\t-bd <filter>      ");
	setColor(255, 255, 255);
	print("Will remove the provided filter from directories the blacklist\n");

	setColor(255, 150, 50);
	print("\t-bd *             ");
	setColor(255, 255, 255);
	print("Will clear the directories blacklist\n");
}

// #####################################################################################################

// Private functions

void sMerge::performRun(bool _difOnly, aci::aDir _d1, aci::aDir _d2) {
	QString prefix;
	if (_difOnly) { prefix = "[merge] [dif]: "; }
	else { prefix = "[merge] [merge]: "; }

	try {
		
		// Scan

		queuePrint(prefix + "Scanning: Directory 1\n");
		_d1.scanAll();
		queuePrint(prefix + "Scanning: Directory 2\n");
		_d2.scanAll();

		queuePrint(prefix + "Directory 1 information (no filter) loaded with:\n\t- Directories: " +
			QString::number(_d1.subDirCount()) + "\n\t- Files: " + QString::number(_d1.fileCount()) + "\n");

		queuePrint(prefix + "Directory 2 information (no filter) loaded with:\n\t- Directories: " +
			QString::number(_d2.subDirCount()) + "\n\t- Files: " + QString::number(_d2.fileCount()) + "\n");

		// #########################################################################################################################

		// Apply filter

		if (m_whiteListDActive) {
			queuePrint(prefix + "Applying filter: Directory: Whitelist: Directory 1\n");
			_d1.filterDirectoriesWithWhitelist(m_whiteListD);
			queuePrint(prefix + "Applying filter: Directory: Whitelist: Directory 2\n");
			_d2.filterDirectoriesWithWhitelist(m_whiteListD);
		}
		if (m_blackListDActive) {
			queuePrint(prefix + "Applying filter: Directory: Blacklist: Directory 1\n");
			_d1.filterDirectoriesWithBlacklist(m_blackListD);
			queuePrint(prefix + "Applying filter: Directory: Blacklist: Directory 2\n");
			_d2.filterDirectoriesWithBlacklist(m_blackListD);
		}
		if (m_whiteListFActive) {
			queuePrint(prefix + "Applying filter: File: Whitelist: Directory 1\n");
			_d1.filterFilesWithWhitelist(m_whiteListF);
			queuePrint(prefix + "Applying filter: File: Whitelist: Directory 2\n");
			_d2.filterFilesWithWhitelist(m_whiteListF);
		}
		if (m_blackListFActive) {
			queuePrint(prefix + "Applying filter: File: Blacklist: Directory 1\n");
			_d1.filterFilesWithBlacklist(m_blackListF);
			queuePrint(prefix + "Applying filter: File: Blacklist: Directory 2\n");
			_d2.filterFilesWithBlacklist(m_blackListF);
		}

		QMetaObject::invokeMethod(this, "print", Qt::QueuedConnection, Q_ARG(const QString&,
			prefix + "Directory 1 information filtered with remaining:\n\t- Directories: " +
			QString::number(_d1.subDirCount()) + "\n\t- Files: " + QString::number(_d1.fileCount()) + "\n"));

		QMetaObject::invokeMethod(this, "print", Qt::QueuedConnection, Q_ARG(const QString&,
			prefix + "Directory 2 information filtered with remaining:\n\t- Directories: " +
			QString::number(_d2.subDirCount()) + "\n\t- Files: " + QString::number(_d2.fileCount()) + "\n"));
		
		// #########################################################################################################################

		// Check dirs
		
		queuePrint(prefix + "Check directories\n");
		if (checkDirectories(_difOnly, _d1, _d2, prefix) && (_difOnly || !m_autoMerge)) {
			queueColor(QColor(255, 150, 50));
			queuePrint(prefix + "! > Execution stopped. Continue manually\n");
			QMetaObject::invokeMethod(this, "slotFinishRun", Qt::QueuedConnection, Q_ARG(bool, _difOnly));
			return;
		}
		queueColor(QColor(255, 255, 255));
		queuePrint(prefix + "Check directories ");
		queueColor(QColor(0, 255, 0));
		queuePrint("NO DIF\n");
		queueColor(QColor(255, 255, 255));

		// #########################################################################################################################

		// Check files existance

		queuePrint(prefix + "Check files (existance)\n");
		if (checkFiles(_difOnly, _d1, _d2, prefix) && (_difOnly || !m_autoMerge)) {
			queueColor(QColor(255, 150, 50));
			queuePrint(prefix + "! > Execution stopped. Continue manually\n");
			QMetaObject::invokeMethod(this, "slotFinishRun", Qt::QueuedConnection, Q_ARG(bool, _difOnly));
			return;
		}
		queueColor(QColor(255, 255, 255));
		queuePrint(prefix + "Check files (existance) ");
		queueColor(QColor(0, 255, 0));
		queuePrint("NO DIF\n");
		queueColor(QColor(255, 255, 255));

		// #########################################################################################################################

		// Check file content

		queuePrint(prefix + "Check files (content)\n");
		if (checkFileContent(_difOnly, _d1, _d2, prefix)) {
			queueColor(QColor(255, 255, 255));
			queuePrint(prefix + "Check files (content) ");
			queueColor(QColor(0, 255, 0));
			queuePrint("DONE\n");
			queueColor(QColor(255, 255, 255));
		}
		else {
			queueColor(QColor(255, 255, 255));
			queuePrint(prefix + "Check files (content) ");
			queueColor(QColor(0, 255, 0));
			queuePrint("NO DIF\n");
			queueColor(QColor(255, 255, 255));
		}
	}
	catch (const std::exception& _e) {
		QMetaObject::invokeMethod(this, "slotFinishRun", Qt::QueuedConnection, Q_ARG(bool, _difOnly));
		queuePrint(prefix + "Check directories\n");
	}
	QMetaObject::invokeMethod(this, "slotFinishRun", Qt::QueuedConnection, Q_ARG(bool, _difOnly));
}

bool sMerge::checkDirectories(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const QString& _logPrefix) {

	bool ret{ false };

	std::list<QString> sub1 = _d1.subdirectoryNameList();
	std::list<QString> sub2 = _d2.subdirectoryNameList();

	// ##### 1 > 2

	queueColor(QColor(255, 150, 50));
	queuePrint(_logPrefix + "##############################################################\n");
	queueColor(QColor(255, 255, 255));
	queuePrint(_logPrefix + "     From: " + _d1.fullPath() + "\n" +
		_logPrefix + "     To:   " + _d2.fullPath() + "\n\n");

	for (auto s1 : sub1) {
		bool found{ false };
		for (auto s2 : sub2) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(QColor(255, 0, 0));
				queuePrint(_logPrefix + "Missing: " + s1 + "\n");
			}
			else {
				queueColor(QColor(255, 150, 50));
				queuePrint(_logPrefix + "Creating: " + s1 + "\n");
				QDir().mkdir(_d2.fullPath() + "/" + s1);
			}
			ret = true;
		}
	}

	// ##### 2 > 1

	queueColor(QColor(255, 150, 50));
	queuePrint(_logPrefix + "##############################################################\n");
	queueColor(QColor(255, 255, 255));
	queuePrint(_logPrefix + "     From: " + _d2.fullPath() + "\n" +
		_logPrefix + "     To:   " + _d1.fullPath() + "\n\n");

	for (auto s2 : sub2) {
		bool found{ false };
		for (auto s1 : sub1) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(QColor(255, 0, 0));
				queuePrint(_logPrefix + "Missing: " + s2 + "\n");
			}
			else {
				queueColor(QColor(255, 150, 50));
				queuePrint(_logPrefix + "Creating: " + s2 + "\n");
				if (!QDir().mkdir(_d1.fullPath() + "/" + s2)) {
					queueColor(QColor(255, 0, 0));
					queuePrint(_logPrefix + "Failed to create directory (Auto merge disabled now)\n");
					m_autoMerge = false;
					return true;
				}
			}
			ret = true;
		}
	}

	return ret;
}

bool sMerge::checkFiles(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const QString& _logPrefix) {

	bool ret{ false };

	std::list<QString> files1 = _d1.fileNameList();
	std::list<QString> files2 = _d2.fileNameList();

	// ##### 1 > 2

	queueColor(QColor(255, 150, 50));
	queuePrint(_logPrefix + "##############################################################\n");
	queueColor(QColor(255, 255, 255));
	queuePrint(_logPrefix + "     From: " + _d1.fullPath() + "\n" +
		_logPrefix + "     To:   " + _d2.fullPath() + "\n\n");

	for (auto s1 : files1) {
		bool found{ false };
		for (auto s2 : files2) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(QColor(255, 0, 0));
				queuePrint(_logPrefix + "Missing: " + s1 + "\n");
			}
			else {
				queueColor(QColor(255, 150, 50));
				queuePrint(_logPrefix + "Creating: " + s1 + "\n");
				
				QFile file(_d1.fullPath() + "/" + s1);
				if (!file.copy(_d2.fullPath() + "/" + s1)) {
					queueColor(QColor(255, 0, 0));
					queuePrint(_logPrefix + "Failed to copy file (Auto merge disabled now)\n");
					m_autoMerge = false;
					return true;
				}
			}
			ret = true;
		}
	}

	// ##### 2 > 1

	queueColor(QColor(255, 150, 50));
	queuePrint(_logPrefix + "##############################################################\n");
	queueColor(QColor(255, 255, 255));
	queuePrint(_logPrefix + "     From: " + _d2.fullPath() + "\n" +
		_logPrefix + "     To:   " + _d1.fullPath() + "\n\n");

	for (auto s2 : files2) {
		bool found{ false };
		for (auto s1 : files1) { if (s1 == s2) { found = true; break; } }
		if (!found) {
			if (_difOnly) {
				queueColor(QColor(255, 0, 0));
				queuePrint(_logPrefix + "Missing: " + s2 + "\n");
			}
			else {
				queueColor(QColor(255, 150, 50));
				queuePrint(_logPrefix + "Creating: " + s2 + "\n");

				QFile file(_d2.fullPath() + "/" + s2);
				if (!file.copy(_d1.fullPath() + "/" + s2)) {
					queueColor(QColor(255, 0, 0));
					queuePrint(_logPrefix + "Failed to copy file (Auto merge disabled now)\n");
					m_autoMerge = false;
					return true;
				}
			}
			ret = true;
		}
	}

	return ret;
}

bool sMerge::checkFileContent(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const QString& _logPrefix) {
	bool ret{ false };

	std::list<QString> files1 = _d1.fileNameList();
	std::list<QString> files2 = _d2.fileNameList();

	// ##### 1 > 2

	queueColor(QColor(255, 150, 50));
	queuePrint(_logPrefix + "##############################################################\n");
	queueColor(QColor(255, 255, 255));
	queuePrint(_logPrefix + _d1.fullPath() + "\n");
	queuePrint(_logPrefix + "                         " + _d2.fullPath() + "\n");

	for (auto f1 : files1) {
		// File 1
		QFile file1(_d1.fullPath() + "/" + f1);
		if (!file1.exists()) {
			queueColor(QColor(255, 0, 0));
			queuePrint(_logPrefix + "File does not exist (" + _d1.fullPath() + "/" + f1 + ")\n");
			return true;
		}
		if (!file1.open(QIODevice::ReadOnly)) {
			queueColor(QColor(255, 0, 0));
			queuePrint(_logPrefix + "Failed to open file for reading (" + _d1.fullPath() + "/" + f1 + ")\n");
			return true;
		}

		QByteArray data1 = file1.readAll();
		file1.close();

		// File 2
		QFile file2(_d2.fullPath() + "/" + f1);
		if (!file2.exists()) {
			queueColor(QColor(255, 0, 0));
			queuePrint(_logPrefix + "File does not exist (" + _d2.fullPath() + "/" + f1 + ")\n");
			return true;
		}
		if (!file2.open(QIODevice::ReadOnly)) {
			queueColor(QColor(255, 0, 0));
			queuePrint(_logPrefix + "Failed to open file for reading (" + _d2.fullPath() + "/" + f1 + ")\n");
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
					queueColor(QColor(0, 255, 0));
					queuePrint(_logPrefix + f1 + " [NEWER]\n");
				}
				else {
					// Delete old file
					if (!file2.remove()) {
						queueColor(QColor(255, 0, 0));
						queuePrint(_logPrefix + "Failed to delete outdated file (" + _d2.fullPath() + "/" + f1 + ")\n");
						return true;
					}

					// Copy newer file
					if (!file1.copy(_d2.fullPath() + "/" + f1)) {
						queueColor(QColor(255, 0, 0));
						queuePrint(_logPrefix + "Failed to copy file (" + _d1.fullPath() + "/" + f1 + "  ->  " + _d2.fullPath() + "/" + f1 + ")\n");
						return true;
					}

					queueColor(QColor(255, 0, 0));
					queuePrint(_logPrefix + "                         Updated: " + _d2.fullPath() + "/" + f1 + "\n");
				}
			}
			else if (info1.lastModified() < info2.lastModified()) {
				if (_difOnly) {
					queueColor(QColor(0, 255, 0));
					queuePrint(_logPrefix + "                         " + f1 + " [NEWER]\n");
				}
				else {
					// Delete old file
					if (!file1.remove()) {
						queueColor(QColor(255, 0, 0));
						queuePrint(_logPrefix + "Failed to delete outdated file (" + _d1.fullPath() + "/" + f1 + ")\n");
						return true;
					}

					// Copy newer file
					if (!file2.copy(_d1.fullPath() + "/" + f1)) {
						queueColor(QColor(255, 0, 0));
						queuePrint(_logPrefix + "Failed to copy file (" + _d2.fullPath() + "/" + f1 + "  ->  " + _d1.fullPath() + "/" + f1 + ")\n");
						return true;
					}

					queueColor(QColor(255, 0, 0));
					queuePrint(_logPrefix + "Updated: " + _d1.fullPath() + "/" + f1 + "\n");
				}
			}
			else {
				queueColor(QColor(255, 150, 50));
				queuePrint(_logPrefix + "Can not merge \"" + f1 + "\"\n");
				queuePrint(_logPrefix + "       ^-- Files differ with same last modified value\n");
				if (!_difOnly) { return true; }
			}
			ret = true;
		}

	}

	return ret;
}
