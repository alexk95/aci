#pragma once

#include "merge_global.h"
#include <aci/InterpreterObject.h>
#include <aci/aDir.h>

#include <qstring.h>

#include <list>

class AppBase;

class sMerge : public aci::InterpreterObject {
public:
	sMerge(AppBase * _app);
	virtual ~sMerge();

	virtual std::wstring key(void) const override { return L"merge"; }

	virtual bool handle(const std::wstring& _command, const std::list<std::wstring>& _params) override;

	// #####################################################################################################

	// Commands

	void cmdConfiguration(void);

	void cmdClear(void);

	bool cmdLoad(const QString& _filename);

	bool cmdSave(const QString& _filename);

	void showSavedConfigs(void);

	// #####################################################################################################

	// Merger

	bool run(bool _difOnly);

	void finishRun(bool _difOnly);

protected:
	virtual void showCommandInfo(void) override;

private:

	void performRun(bool _difOnly, aci::aDir _d1, aci::aDir _d2);

	bool checkDirectories(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix);

	bool checkFiles(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix);

	bool checkFileContent(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const std::wstring& _logPrefix);

	AppBase *				m_app;
	std::wstring			m_pathOne;
	std::wstring			m_pathTwo;
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
};
