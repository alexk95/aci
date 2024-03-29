#pragma once

#include "merge_global.h"
#include <aci/InterpreterObject.h>
#include <aci/aDir.h>

#include <qstring.h>

#include <list>

class MERGE_EXPORT merge : public aci::InterpreterObject {
public:
	merge(void);
	virtual ~merge();

	virtual std::wstring key(void) const override { return L"merge"; }

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

	bool run(bool _diffOnly);

	void finishRun(bool _diffOnly);

protected:
	virtual void showCommandInfo(void) override;

private:

	void performRun(bool _diffOnly, aci::aDir _d1, aci::aDir _d2);

	bool checkDirectories(bool _diffOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix);

	bool checkFiles(bool _diffOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix);

	bool checkFileContent(bool _diffOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix);

	std::wstring			m_pathOne;
	std::wstring			m_pathTwo;
	std::wstring			m_prefix;
	bool					m_searchTopDirectoryOnly;
	bool					m_autoMerge;

	bool					m_blackListDActive;
	bool					m_whiteListDActive;
	std::list<std::wstring>	m_blackListD;
	std::list<std::wstring>	m_whiteListD;

	bool					m_blackListFActive;
	bool					m_whiteListFActive;
	std::list<std::wstring>	m_blackListF;
	std::list<std::wstring>	m_whiteListF;

	merge(merge&) = delete;
	merge& operator = (merge&) = delete;
};
