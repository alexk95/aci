#pragma once

#include "source_global.h"
#include <aci/InterpreterObject.h>
#include <aci/aDir.h>

#include <qstring.h>

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

	void finishRun();

protected:
	virtual void showCommandInfo(void) override;

private:

	void performRun(aci::aDir _dir, RunMode _mode);

	void scanDirectories(const std::wstring& _pathPrefix, const aci::aDir& _dir);

	void scanFilenames(const std::wstring& _pathPrefix, const aci::aDir& _dir);

	void scanFiles(const aci::aDir& _dir, unsigned long long& _files, unsigned long long& _textLines, unsigned long long& _nonEmptyLines, unsigned long long& _sourceLines);

	void checkSyntaxCPP(const QString& _line, bool& _isComment, unsigned long long& _sourceLines);

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
