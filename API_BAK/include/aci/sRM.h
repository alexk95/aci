#pragma once

// aci header
#include <aci/InterpreterObject.h>

// Qt header
#include <qstringlist.h>

class sRM : public aci::InterpreterObject {
public:
	sRM();
	virtual ~sRM();

	virtual QString key(void) const override { return "rm"; }

	virtual bool handle(const QString& _command, const QStringList& _params) override;

	// #####################################################################################################

protected:
	virtual void showCommandInfo(void) override;

private:
	void run(QStringList& _dataToRemove, bool _dirs, bool _subdirs);

};