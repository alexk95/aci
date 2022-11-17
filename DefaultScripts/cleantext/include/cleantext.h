#pragma once

#include "cleantext_global.h"
#include <aci/InterpreterObject.h>

#include <list>
#include <string>

class CLEANTEXT_API_EXPORT cleantext : public aci::InterpreterObject {
public:
	cleantext(void);
	virtual ~cleantext();

	virtual std::wstring key(void) const override { return L"cleantext"; }

	virtual bool handle(const std::wstring& _command, const std::vector<std::wstring>& _params) override;

	// #####################################################################################################

	// Run
	bool run(const std::wstring& _filename, const std::list<std::wstring>& _prefixRemove, const std::wstring& _newPrefix, const std::list<std::wstring>& _suffixRemove, const std::wstring& _newSuffix, bool _removeEmptyLines, bool _addQuotation, bool _toLower, bool _toUpper, bool _linesUnique);

protected:
	virtual void showCommandInfo(void) override;

private:
	std::wstring		m_prefix;

	cleantext(cleantext&) = delete;
	cleantext& operator = (cleantext&) = delete;
};
