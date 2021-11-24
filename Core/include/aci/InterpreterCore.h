#pragma once

#ifdef ACI_EXPORTS
#pragma warning(disable:4251)
#endif // ACI_EXPORTS

#include <aci/aciSharedDataTypes.h>

#include <map>
#include <list>
#include <string>

namespace aci {
	class AbstractInterpreterNotifier;
	class AbstractPrinter;
	class InterpreterObject;

	class ACI_API_EXPORT InterpreterCore {
	public:
		static InterpreterCore * instance(void);
		static void clearInstance(void);

		// ########################################################################################

		// Setter

		void attachNotifier(AbstractInterpreterNotifier * _notifier) { m_notifier = _notifier; }
		void setAutoClean(bool _isAutoClean) { m_autoClean = _isAutoClean; }
		void attachPrinter(aci::AbstractPrinter * _printer);
		void addScriptObject(aci::InterpreterObject * _obj);
		void setCurrentPath(const std::wstring& _path);

		// ########################################################################################

		// Event handler

		bool handle(const std::wstring& _message);
		bool cmdData(const std::list<std::wstring>& _args);
		void showHelp(void);
		bool cmdCd(const std::wstring& _path);

		// ########################################################################################

		// Getter

		std::wstring currentPath(void) const;
		aci::AbstractPrinter * printer(void) { return m_printer; }

		// ########################################################################################

		// Static functions

		static void extractClassicSyntax(std::list<std::wstring>& _dest, const std::wstring& _origin);

		// ########################################################################################

		// Getter

		const std::map<std::wstring, aci::InterpreterObject *>& objects(void) const { return m_objects; }
		std::map<std::wstring, aci::InterpreterObject *>& objects(void) { return m_objects; }

		aci::InterpreterObject * findFirstMatchingItem(const std::wstring& _key);

	private:
		aci::AbstractPrinter *								m_printer;
		AbstractInterpreterNotifier *						m_notifier;
		bool												m_autoClean;

		std::list<std::wstring>								m_path;

		std::map<std::wstring, aci::InterpreterObject *>	m_objects;

		InterpreterCore();
		virtual ~InterpreterCore();
		InterpreterCore(InterpreterCore&) = delete;
		InterpreterCore& operator = (InterpreterCore&) = delete;
	};
}