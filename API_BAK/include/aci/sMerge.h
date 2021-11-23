#pragma once

#include <aci/InterpreterObject.h>
#include "aDir.h"

#include <qobject.h>
#include <qstring.h>

#include <list>

class AppBase;

class sMerge : public aci::InterpreterObject {
	Q_OBJECT
public:
	sMerge(AppBase * _app);
	virtual ~sMerge();

	virtual QString key(void) const override { return "merge"; }

	virtual bool handle(const QString& _command, const QStringList& _params) override;

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

private slots:
	void slotFinishRun(bool _difOnly);

protected:
	virtual void showCommandInfo(void) override;

private:

	void performRun(bool _difOnly, aci::aDir _d1, aci::aDir _d2);

	bool checkDirectories(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const QString& _logPrefix);

	bool checkFiles(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const QString& _logPrefix);

	bool checkFileContent(bool _difOnly, aci::aDir& _d1, aci::aDir& _d2, const QString& _logPrefix);

	AppBase *				m_app;
	QString					m_pathOne;
	QString					m_pathTwo;
	bool					m_searchTopDirectoryOnly;
	bool					m_autoMerge;

	bool					m_blackListDActive;
	bool					m_whiteListDActive;
	std::list<QString>		m_blackListD;
	std::list<QString>		m_whiteListD;

	bool					m_blackListFActive;
	bool					m_whiteListFActive;
	std::list<QString>		m_blackListF;
	std::list<QString>		m_whiteListF;	
};
