#pragma once

#include "default_global.h"
#include <aci/InterpreterObject.h>
#include <aci/aDir.h>

#include <qstring.h>
#include <qstringlist.h>

#include <list>

class ODBC_LIB_EXPORT odbc : public aci::InterpreterObject {
public:
	enum Configuration {
		NONE,
		EXCEL
	};

	odbc(void);
	virtual ~odbc();

	virtual std::wstring key(void) const override { return L"odbc"; }

	virtual bool handle(const std::wstring& _command, const std::vector<std::wstring>& _params) override;
	
protected:
	virtual void showCommandInfo(void) override;

private:

	void printPrefix(void);

	void showConfiguration(void);

	bool executeQuery(const QString& _query);

	Configuration		m_configuration;
	QString				m_driver;
	QString				m_file;
	int					m_columns;

	odbc(odbc&) = delete;
	odbc& operator = (odbc&) = delete;
};
