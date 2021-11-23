#pragma once

#include <aci/aciSharedDataTypes.h>
#include <aci/InterpreterObject.h>
#include <aci/Color.h>

#include <list>
#include <string>

namespace aci {
	class AbstractPrinter;

	class ACI_API_EXPORT InterpreterObject {
	public:
		InterpreterObject();
		virtual ~InterpreterObject() {}

		virtual std::wstring key(void) const = 0;

		virtual bool handle(const std::wstring& _command, const std::list<std::wstring>& _params) = 0;

		// ################################################################################################################################

		// Output

		void showInfo(void);

		void showDelimiterLine(void);

		// ################################################################################################################################

		// File operations

		bool readDataFile(std::list<std::wstring>& _data, const std::wstring& _filename, bool _showLog);

		bool writeDataFile(const std::list<std::wstring>& _data, const std::wstring& _filename, bool _showLog);

		// ################################################################################################################################

		// Setter

		void attachPrinter(AbstractPrinter * _printer) { m_printer = _printer; }

		void print(const char * _message);
		void print(const wchar_t * _message);
		void queuePrint(const char * _message);
		void queuePrint(const wchar_t * _message);

		void print(bool _value);
		void queuePrint(bool _value);

		void print(const std::string& _message);
		void print(const std::wstring& _message);
		void queuePrint(const std::string& _message);
		void queuePrint(const std::wstring& _message);

		void setColor(int _r, int _g, int _b, int _a = 255);
		void queueColor(int _r, int _g, int _b, int _a = 255);

		void setColor(const Color& _color);
		void queueColor(const Color& _color);

		// ################################################################################################################################

		// Getter

		std::list<std::wstring> filesInDirectory(const std::wstring& _subdirectory);

		std::list<std::wstring> subdirectories(const std::wstring& _subdirectory);

		// ################################################################################################################################

		// Static functions

		static std::wstring isolateFilename(const std::wstring& _path);
		
	protected:
		virtual void showCommandInfo(void) = 0;

		AbstractPrinter *		m_printer;
	};
}