#include "InterpreterCore.h"
#include "AbstractInterpreterNotifier.h"
#include <aci/InterpreterObject.h>
#include <aci/AbstractPrinter.h>

#include <qdir.h>
#include <qfile.h>

#include <aDir.h>
#include <aFile.h>

InterpreterCore * g_instance{ nullptr };

InterpreterCore * InterpreterCore::instance(void) {
	if (g_instance == nullptr) { g_instance = new InterpreterCore; }
	return g_instance;
}

void InterpreterCore::clearInstance(void) {
	if (g_instance) {
		delete g_instance;
		g_instance = nullptr;
	}
}

// ########################################################################################

// Setter

void InterpreterCore::attachPrinter(aci::AbstractPrinter * _printer) {
	m_printer = _printer;
	for (auto obj : m_objects) { obj.second->attachPrinter(m_printer); }
}

void InterpreterCore::addScriptObject(aci::InterpreterObject * _obj) {
	if (_obj == nullptr) {
		if (m_printer) { m_printer->print("Failed to load NULL as script\n"); }
		return;
	}
	auto h = m_objects.find(_obj->key());
	if (h != m_objects.end()) {
		if (m_printer) { m_printer->print("Script \"" + _obj->key() + "\" was already loaded\n"); }
		return;
	}
	m_objects.insert_or_assign(_obj->key(), _obj);
}

void InterpreterCore::setCurrentPath(const QString& _path) {
	m_path.clear();
	if (_path.isEmpty()) { return; }
	QString sub(_path);
	sub = sub.replace("\\", "/");
	QStringList lst = sub.split("/", QString::SkipEmptyParts);
	for (auto s : lst) { m_path.push_back(s); }
}

// ########################################################################################

// Event handler

bool InterpreterCore::handle(const QString& _message) {
	QStringList lst;
	extractClassicSyntax(lst, _message);

	if (_message == "?") {
		showHelp();
		return true;
	}
	else if (_message == "exit" || _message == "end") {
		if (m_printer) { m_printer->print("Shutdown requested\n"); }
		if (m_notifier) { m_notifier->shutdown(); return true; }
		else if (m_printer) {
			m_printer->setColor(QColor(255, 0, 0));
			m_printer->print("No notifier attached\n");
		}
		return false;
	}
	else if (_message == "?exit" || _message == "?end") {
		if (m_printer) {
			m_printer->print("The command \"exit\" or \"end\" will close the application\n");
		}
		return true;
	}
	else if (_message.startsWith("$data")) {
		return cmdData(lst);
	}
	else if (lst.front() == "cd") {
		if (lst.count() == 1) {
			return cmdCd("~");
		}
		else if (lst.count() > 2 || lst.count() < 2) {
			if (m_printer) {
				m_printer->setColor(QColor(255, 0, 0));
				m_printer->print("Invalid nuber of arguments for \"cd\"\n");
			}
			return false;
		}
		else {
			return cmdCd(lst.back());
		}
	}
	else if (_message == "ls") {
		if (m_printer == nullptr) { return true; }
		if (m_path.empty()) {
			m_printer->print("\t<empty>\n");
			return true;
		}
		aci::aDir dir("", currentPath());
		dir.scanDirectories();
		dir.scanFiles();

		m_printer->print(dir.fullPath() + "\n");

		m_printer->setColor(QColor(255, 150, 50));
		for (auto d : dir.subdirectories()) {
			m_printer->print("\t[DIR]:  " + d->name() + "\n");
		}
		m_printer->setColor(QColor(50, 255, 150));
		for (auto f : dir.files()) {
			m_printer->print("\t[FILE]: " + f->name() + "\n");
		}
		return true;
	}
	else if (lst.count() == 1 && lst.front().startsWith("?")) {
		QString key = lst.front();
		key.remove(0, 1);
		auto handler = m_objects.find(key);
		if (handler != m_objects.end()) {
			handler->second->showInfo();
			return true;
		}
		else if (m_printer) {
			m_printer->setColor(QColor(255, 0, 0));
			m_printer->print("Command \"");
			m_printer->setColor(QColor(200, 200, 0));
			m_printer->print(key);
			m_printer->setColor(QColor(255, 0, 0));
			m_printer->print("\" was not recognized\n");
		}
		return false;
	}
	else {
		auto handler = m_objects.find(lst.front());
		if (handler != m_objects.end()) {
			return handler->second->handle(_message, lst);
		} else if (m_printer) { 
			m_printer->setColor(QColor(255, 0, 0));
			m_printer->print("Command \"");
			m_printer->setColor(QColor(200, 200, 0));
			m_printer->print(lst.front());
			m_printer->setColor(QColor(255, 0, 0));
			m_printer->print("\" was not recognized\n");
		}
		return false;
	}
}

bool InterpreterCore::cmdData(const QStringList& _args) {
	if (_args.empty()) {
		if (m_printer) { m_printer->print("[ERROR] [INTERPRETER] [CMD]: Invalid number of arguments\n"); }
		return false;
	}
	if (_args.count() == 1) {
		if (m_printer) {
			m_printer->print("[INTERPRETER] [DATA]: Registered commands\n");
			m_printer->print("\t?\n");
			m_printer->print("\t$data\n");
			m_printer->print("\texit\n\tend\n");
			m_printer->print("\tcd\n");
			for (auto obj : m_objects) {
				m_printer->print("\t" + obj.first + "\n");
			}
		}
		return true;
	}
	else {
		if (m_printer) { m_printer->print("[ERROR] [INTERPRETER] [CMD]: Invalid number of arguments\n"); }
		return false;
	}
}

void InterpreterCore::showHelp(void) {
	if (m_printer) {
		m_printer->print("################################\n");
		m_printer->print("Core commands\n");
		m_printer->print("\t$data		display all datasets\n");
		m_printer->print("\t?<command>  display the command help\n");
		m_printer->print("################################\n");
	}
}

bool InterpreterCore::cmdCd(const QString& _path) {
	if (_path == "?") {
		if (m_printer) {
			m_printer->print("########################################\n## cd ##\n\n");
			m_printer->print("\tcd ?                 Show this help\n");
			m_printer->print("\tcd ~  -OR-  cd       Show the current configuration\n");
			m_printer->print("\tcd <directory>       Change the directory to the directory specified\n");
			m_printer->print("\tcd -                 Go up one directory\n");
			m_printer->print("\tcd ??                Print the current directory\n");
		}
		return true;
	}
	if (_path.isEmpty() || _path == "~") {
		m_path.clear();
		return true;
	}
	else if (_path == "-") {
		if (!m_path.empty()) { m_path.pop_back(); }
		return true;
	}
	else if (_path == "??") {
		if (m_printer) {
			m_printer->print(currentPath() + "\n");
		}
		return true;
	}
	else {
		if (!m_path.empty()) {
			QString subDir = currentPath() + "/" + _path;
			QDir currentDir(subDir);
			if (currentDir.exists()) {
				setCurrentPath(currentPath() + "/" + _path);
				return true;
			}
		}

		QDir dir(_path);
		if (dir.exists()) {
			setCurrentPath(_path);
			return true;
		}

		if (m_printer) {
			m_printer->setColor(QColor(255, 0, 0));
			m_printer->print("Directory not found\n");
		}

		return false;
	}
}

// ########################################################################################

// Getter

QString InterpreterCore::currentPath(void) const {
	if (m_path.empty()) { return "root"; }
	QString path;
	for (QString p : m_path) {
		if (!path.isEmpty()) { path.append("/"); }
		path.append(p);
	}
	return path;
}

aci::InterpreterObject * InterpreterCore::findFirstMatchingItem(const QString& _key) {
	auto it = m_objects.find(_key);
	if (it != m_objects.end()) {
		return it->second;
	}
	for (auto it : m_objects) {
		if (it.second->key().startsWith(_key)) { return it.second; }
	}
	return nullptr;
}

// ########################################################################################

// Static functions

void InterpreterCore::extractClassicSyntax(QStringList& _dest, const QString& _origin) {
	bool isString{ false };
	QString temp;
	for (QChar c : _origin) {
		if (c == ' ') {
			if (isString) { temp.append(c); }
			else { _dest.push_back(temp); temp.clear(); }
		}
		else if (c == '\"') { isString = !isString; }
		else { temp.append(c); }
	}
	if (!temp.isEmpty()) { _dest.push_back(temp); }
}

// ########################################################################################

// Private functions

InterpreterCore::InterpreterCore()
	: m_printer(nullptr), m_autoClean(true), m_notifier(nullptr)
{
	setCurrentPath(QDir::currentPath());
}

InterpreterCore::~InterpreterCore() {
	if (m_autoClean) {
		for (auto obj : m_objects) { delete obj.second; }
	}
}