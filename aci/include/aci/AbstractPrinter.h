#pragma once

#include <aci/aciSharedDataTypes.h>
#include <aci/Color.h>

#include <string>

namespace aci {
	class ACI_API_EXPORT AbstractPrinter {
	public:
		AbstractPrinter() {}
		virtual ~AbstractPrinter() {}
		
		virtual void print(const std::string& _str) = 0;
		virtual void print(const std::wstring& _str) = 0;
		virtual void printAsync(const std::string& _str) = 0;
		virtual void printAsync(const std::wstring& _str) = 0;
		virtual void setColor(const aci::Color& _color) = 0;
		virtual void setColorAsync(const aci::Color& _color) = 0;

		void print(const char * _str) { print(std::string(_str)); }
		void print(const wchar_t * _str) { print(std::wstring(_str)); }
		void printAsync(const char * _str) { printAsync(std::string(_str)); }
		void printAsync(const wchar_t * _str) { printAsync(std::wstring(_str)); }
		void setColor(int _r, int _g, int _b, int _a = 255) { setColor(aci::Color(_r, _g, _b, _a)); }
		void setColorAsync(int _r, int _g, int _b, int _a = 255) { setColorAsync(aci::Color(_r, _g, _b, _a)); }
	};
}