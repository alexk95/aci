#pragma once

#include <qstring.h>
#include <qstringlist.h>

#include <map>
#include <list>

class AbstractInterpreterNotifier;

namespace aci {
	class AbstractPrinter;
	class InterpreterObject;
}

class InterpreterCore {
public:
	static InterpreterCore * instance(void);
	static void clearInstance(void);

	// ########################################################################################

	// Setter

	void attachNotifier(AbstractInterpreterNotifier * _notifier) { m_notifier = _notifier; }
	void setAutoClean(bool _isAutoClean) { m_autoClean = _isAutoClean; }
	void attachPrinter(aci::AbstractPrinter * _printer);
	void addScriptObject(aci::InterpreterObject * _obj);
	void setCurrentPath(const QString& _path);

	// ########################################################################################

	// Event handler
	
	bool handle(const QString& _message);
	bool cmdData(const QStringList& _args);
	void showHelp(void);
	bool cmdCd(const QString& _path);

	// ########################################################################################

	// Getter
	
	QString currentPath(void) const;
	aci::AbstractPrinter * printer(void) { return m_printer; }

	// ########################################################################################

	// Static functions

	static void extractClassicSyntax(QStringList& _dest, const QString& _origin);

	// ########################################################################################

	// Getter

	const std::map<QString, aci::InterpreterObject *>& objects(void) const { return m_objects; }
	std::map<QString, aci::InterpreterObject *>& objects(void) { return m_objects; }

	aci::InterpreterObject * findFirstMatchingItem(const QString& _key);

private:
	aci::AbstractPrinter *							m_printer;
	AbstractInterpreterNotifier *					m_notifier;
	bool											m_autoClean;

	std::list<QString>								m_path;

	std::map<QString, aci::InterpreterObject *>	m_objects;

	InterpreterCore();
	virtual ~InterpreterCore();
	InterpreterCore(InterpreterCore&) = delete;
	InterpreterCore& operator = (InterpreterCore&) = delete;
};