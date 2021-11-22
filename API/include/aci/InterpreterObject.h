#pragma once

#include <aci/aciAPIsharedTypes.h>
#include <aci/InterpreterObject.h>

#include <qobject.h>
#include <qstring.h>
#include <qcolor.h>
#include <qbytearray.h>
#include <qstringlist.h>

namespace aci {
	class AbstractPrinter;

	class ACI_API_EXPORT InterpreterObject : public QObject {
		Q_OBJECT
	public:
		InterpreterObject();
		virtual ~InterpreterObject() {}

		virtual QString key(void) const = 0;

		virtual bool handle(const QString& _command, const QStringList& _params) = 0;

		// ################################################################################################################################

		// Output

		void showInfo(void);

		void showDelimiterLine(void);

		// ################################################################################################################################

		// File operations

		bool readDataFile(QByteArray& _data, const QString& _filename, bool _showLog);

		bool writeDataFile(const QByteArray& _data, const QString& _filename, bool _showLog);

		// ################################################################################################################################

		// Setter

		void attachPrinter(AbstractPrinter * _printer) { m_printer = _printer; }

		void queuePrint(const char * _message);

		void queuePrint(bool _value);

		void queuePrint(const QString& _message);

		void queueColor(const QColor& _color);

		void queueColor(int _r, int _g, int _b, int _a = 255);

		// ################################################################################################################################

		// Getter

		QStringList filesInDirectory(const QString& _subdirectory);

		QStringList subdirectories(const QString& _subdirectory);

		// ################################################################################################################################

		// Static functions

		static QString isolateFilename(const QString& _path);

	public slots:
		void print(const char * _message);

		void print(bool _value);

		void print(const QString& _message);

		void setColor(const QColor& _color);

		void setColor(int _r, int _g, int _b, int _a = 255);

	protected:
		virtual void showCommandInfo(void) = 0;

		AbstractPrinter *		m_printer;
	};
}