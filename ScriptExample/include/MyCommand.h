#pragma once

#include <aci/InterpreterObject.h>

class MyCommand : public aci::InterpreterObject {
public:
	MyCommand();
	virtual ~MyCommand();

	virtual QString key(void) const override;

	virtual bool handle(const QString& _command, const QStringList& _params) override;

protected:
	virtual void showCommandInfo(void) override;

private:
	QByteArray		m_data;
};