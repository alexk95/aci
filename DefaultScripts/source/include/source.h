#pragma once

#include "source_global.h"
#include <aci/InterpreterObject.h>
#include <aci/aDir.h>

#include <qstring.h>

#include <map>
#include <list>

class SOURCE_EXPORT source : public aci::InterpreterObject {
public:
	enum RunMode {
		ALL,
		FILESONLY,
		DIRSONLY
	};

	source(void);
	virtual ~source();

	virtual std::wstring key(void) const override { return L"source"; }

	virtual bool handle(const std::wstring& _command, const std::vector<std::wstring>& _params) override;

	// #####################################################################################################

	// Commands

	void cmdConfiguration(void);

	void cmdClear(void);

	bool cmdLoad(const QString& _filename);

	bool cmdSave(const QString& _filename);

	void showSavedConfigs(void);

	// #####################################################################################################

	// Merger

	bool run(RunMode _mode);

	bool find(const std::wstring& _text);

	bool cleanTab(int _tabLength);

protected:
	virtual void showCommandInfo(void) override;

private:
	void performRun(aci::aDir _dir, RunMode _mode);

	void performFind(aci::aDir _dir, const std::wstring& _text);

	void performCleanTab(aci::aDir _dir, int _tabLength);

	void runScanDirectories(const std::wstring& _pathPrefix, const aci::aDir& _dir);

	void runScanFilenames(const std::wstring& _pathPrefix, const aci::aDir& _dir);

	void runScanFiles(const aci::aDir& _dir, unsigned long long& _files, unsigned long long& _textLines, unsigned long long& _nonEmptyLines, unsigned long long& _sourceLines, std::map<QString, unsigned long long>& _additionalInformation);

	void runCheckSyntaxCPP(const QString& _line, bool& _isComment, unsigned long long& _sourceLines, std::map<QString, unsigned long long>& _additionalInformation);

	void runLineHasKey(const QString& _line, const QString& _key, const QString& _keyName, std::map<QString, unsigned long long>& _informationMap);

	void runCleanTab(const aci::aDir& _dir, int _tabLength);

	void runCleanTab(const aci::aFile& _file, int _tabLength);

	void findScanDirectory(const aci::aDir& _dir, const std::wstring& _shortPath, const QString& _textToFind, std::list<std::pair<std::wstring, unsigned long long>>& _matches);

	void finishPerform();

	void scanAndTidyDir(aci::aDir& _dir);

	static std::wstring fillString(const std::wstring& _original, size_t _length);

	std::wstring			m_path;
	std::wstring			m_prefix;
	bool					m_searchTopDirectoryOnly;
	std::wstring			m_language;

	bool					m_blackListDActive;
	bool					m_whiteListDActive;
	std::list<std::wstring>	m_blackListD;
	std::list<std::wstring>	m_whiteListD;

	bool					m_blackListFActive;
	bool					m_whiteListFActive;
	std::list<std::wstring>	m_blackListF;
	std::list<std::wstring>	m_whiteListF;

	source(source&) = delete;
	source& operator = (source&) = delete;
};
