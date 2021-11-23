#pragma once

#include <aci/InterpreterObject.h>
#include <qbytearray.h>

class sCode : public aci::InterpreterObject {
public:
	sCode();
	virtual ~sCode();

	virtual QString key(void) const override { return "code"; }

	virtual bool handle(const QString& _command, const QStringList& _params) override;

	void cmdImport(const QString& _filename);

	void cmdOutput(const QString& _filename);

	void cmdConvert(void);

	void cmdShow(void);

protected:
	virtual void showCommandInfo(void) override;

private:
	QByteArray		m_data;

};