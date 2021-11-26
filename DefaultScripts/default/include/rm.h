#pragma once

#include "default_global.h"
#include <aci/InterpreterObject.h>
#include <aci/aDir.h>

#include <qstring.h>
#include <qstringlist.h>

#include <list>

class DEFAULT_LIB_EXPORT rm : public aci::InterpreterObject {
public:
	rm(void);
	virtual ~rm();

	virtual std::wstring key(void) const override { return L"rm"; }

	virtual bool handle(const std::wstring& _command, const std::list<std::wstring>& _params) override;
	
protected:
	virtual void showCommandInfo(void) override;

private:

	void clearFiles(aci::aDir& _dir, bool _subs);
	void clearDirs(aci::aDir& _dir, bool _subs);

	bool run(QStringList& _dataToRemove, bool _dirs, bool _files, bool _subdirs);

	rm(rm&) = delete;
	rm& operator = (rm&) = delete;
};
