#pragma once

#include <aci/aciAPIsharedTypes.h>

#include <qstring.h>
#include <qcolor.h>

namespace aci {
	class ACI_API_EXPORT AbstractPrinter {
	public:
		AbstractPrinter() {}
		virtual ~AbstractPrinter() {}
		void print(const char * _str) { print(QString(_str)); }
		virtual void print(const QString& _str) = 0;
		void setColor(int _r, int _g, int _b, int _a = 255) { setColor(QColor(_r, _g, _b, _a)); }
		virtual void setColor(const QColor& _color) = 0;
	};
}