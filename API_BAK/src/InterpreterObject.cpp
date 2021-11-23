#include <aci/InterpreterObject.h>
#include <aci/AbstractPrinter.h>
#include <aci/Settings.h>

#include <qdir.h>
#include <qfile.h>
#include <qstringlist.h>

aci::InterpreterObject::InterpreterObject() : m_printer(nullptr) {}

// ################################################################################################################################

// Output

void aci::InterpreterObject::showInfo(void) {
	showDelimiterLine();
	print("###  ");
	print(key());
	print("  ###\n\n");
	showCommandInfo();
}

void aci::InterpreterObject::showDelimiterLine(void) {
	print("##################################################\n");
}

// ################################################################################################################################

// File operations

bool aci::InterpreterObject::readDataFile(QByteArray& _data, const QString& _filename, bool _showLog) {
	QString filePath = Settings::instance()->dataPath() + "/ScriptData/" + key() + "/" + _filename;
	QFile file(filePath);

	// Check for file exists
	if (_showLog) { print("[IO] [FILE]: Checking file for existance: \n\t" + filePath + "\n\t["); }

	if (file.exists()) {
		if (_showLog) {
			setColor(0, 255, 0);
			print("TRUE");
			setColor(255, 255, 255);
			print("]\n");
		}
	}
	else {
		if (_showLog) {
			setColor(255, 0, 0);
			print("FALSE");
			setColor(255, 255, 255);
			print("]\n[IO] [FILE]: Import cancelled\n");
		}
		return false;
	}

	// Try to open file
	if (_showLog) { print("[IO] [FILE]: Try to open file for reading\n\t["); }

	if (file.open(QIODevice::ReadOnly)) {
		if (_showLog) {
			setColor(0, 255, 0);
			print("TRUE");
			setColor(255, 255, 255);
			print("]\n");
		}
	}
	else {
		if (_showLog) {
			setColor(255, 0, 0);
			print("FALSE");
			setColor(255, 255, 255);
			print("]\n[IO] [FILE]: Import cancelled\n");
		}
		return false;
	}

	// Read file
	if (_showLog) { print("[IO] [FILE]: Read file\n\t["); }
	_data = file.readAll();
	if (_showLog) { print(QString::number(_data.count()) + " bytes]\n"); }
	file.close();
	return true;
}

bool aci::InterpreterObject::writeDataFile(const QByteArray& _data, const QString& _filename, bool _showLog) {
	QString filePath = Settings::instance()->dataPath() + "/ScriptData/" + key() + "/" + _filename;
	filePath.replace("\\", "/");
	QFile file(filePath);

	// Check for file exists
	if (_showLog) { print("[IO] [FILE]: Checking file for existance:\n\t" + filePath + "\n\t["); }

	if (file.exists()) {
		if (_showLog) {
			print(true);
			setColor(255, 255, 255);
			print("]\n");
		}
		if (_showLog) { print("[IO] [FILE]: Trying to delete the file\n"); }
		if (!file.remove()) {
			if (_showLog) {
				setColor(255, 0, 0);
				print("\tFAILED\n");
				setColor(255, 255, 255);
			}
			return false;
		}
		if (_showLog) {
			setColor(0, 255, 0);
			print("\tDONE\n");
			setColor(255, 255, 255);
		}
	}
	else {
		if (_showLog) {
			print(false);
			setColor(255, 255, 255);
			print("]\n");
		}
	}

	// Check path
	QStringList pathData = filePath.split("/", QString::SkipEmptyParts);
	QString p;
	for (int i{ 0 }; i < pathData.count() - 1; i++) {
		if (!p.isEmpty()) { p.append("/"); }
		p.append(pathData[i]);
		QDir dir(p);
		if (!dir.exists()) {
			if (!dir.mkdir(p)) {
				if (_showLog) {
					setColor(255, 0, 0);
					print("Failed to create directory\n");
					return false;
				}
			}
		}
	}

	// Try to open file
	if (_showLog) { print("[IO] [FILE]: Try to open file for writing\n\t["); }

	if (file.open(QIODevice::WriteOnly)) {
		if (_showLog) {
			setColor(0, 255, 0);
			print("TRUE");
			setColor(255, 255, 255);
			print("]\n");
		}
	}
	else {
		if (_showLog) {
			setColor(255, 0, 0);
			print("FALSE");
			setColor(255, 255, 255);
			print("]\n[IO] [FILE]: Export cancelled\n");
		}
		return false;
	}

	// Read file
	if (_showLog) { print("[IO] [FILE]: Write file\n\t["); }
	print(QString::number(file.write(_data)) + "]\n");
	file.close();
	return true;
}

// ################################################################################################################################

// Setter

void aci::InterpreterObject::queuePrint(const char * _message) {
	queuePrint(QString(_message));
}

void aci::InterpreterObject::queuePrint(bool _value) {
	QMetaObject::invokeMethod(this, "print", Qt::QueuedConnection, Q_ARG(bool, _value));
}

void aci::InterpreterObject::queuePrint(const QString& _message) {
	QMetaObject::invokeMethod(this, "print", Qt::QueuedConnection, Q_ARG(const QString&, _message));
}

void aci::InterpreterObject::queueColor(const QColor& _color) {
	QMetaObject::invokeMethod(this, "setColor", Qt::QueuedConnection, Q_ARG(const QColor&, _color));
}

void aci::InterpreterObject::queueColor(int _r, int _g, int _b, int _a) {
	queueColor(QColor(_r, _g, _b, _a));
}

// ################################################################################################################################

// Getter

QStringList aci::InterpreterObject::filesInDirectory(const QString& _subdirectory) {
	QString path = Settings::instance()->dataPath() + "/ScriptData/" + key() + "/" + _subdirectory;
	QDir dir(path);
	if (dir.exists()) {
		return dir.entryList(QDir::Filter::Files);
	}
	else {
		return QStringList();
	}
}

QStringList aci::InterpreterObject::subdirectories(const QString& _subdirectory) {
	QString path = Settings::instance()->dataPath() + "/ScriptData/" + key() + "/" + _subdirectory;
	QDir dir(path);
	if (dir.exists()) {
		return dir.entryList(QDir::Filter::Dirs);
	}
	else {
		return QStringList();
	}
}

// ################################################################################################################################

// Static functions

QString aci::InterpreterObject::isolateFilename(const QString& _path) {
	QString path = _path;
	path.replace("\\", "/");
	QStringList lst = path.split("/");
	QStringList filename = lst.back().split(".");
	QString file;
	for (int i{ 0 }; i < filename.count() - 1; i++) {
		if (!file.isEmpty()) { file.append("."); }
		file.append(filename[i]);
	}
	return file;
}

// ################################################################################################################################

// Slots

void aci::InterpreterObject::print(const QString& _message) {
	if (m_printer) { m_printer->print(_message); }
}

void aci::InterpreterObject::print(const char * _message) {
	print(QString(_message));
}

void aci::InterpreterObject::print(bool _value) {
	if (_value) { setColor(0, 255, 0); print("TRUE"); }
	else { setColor(255, 0, 0); print("FALSE"); }
}

void aci::InterpreterObject::setColor(const QColor& _color) {
	if (m_printer) { m_printer->setColor(_color); }
}

void aci::InterpreterObject::setColor(int _r, int _g, int _b, int _a) {
	setColor(QColor(_r, _g, _b, _a));
}

