// aci header
#include <aci/Settings.h>

// uiCore header
#include <akAPI/uiAPI.h>

// Qt header
#include <qdir.h>

using namespace ak;

aci::Settings * g_instance{ nullptr };

aci::Settings *  aci::Settings::instance(void) {
	if (g_instance == nullptr) { g_instance = new Settings; }
	return g_instance;
}

void aci::Settings::clearInstance(void) {
	if (g_instance) {
		delete g_instance;
		g_instance = nullptr;
	}
}

// ######################################################################################################

// Base class functions

bool aci::Settings::handle(const QString& _command, const QStringList& _params) {
	if (_params.count() == 2) {
		if (_params.back() == "cfg") {
			cmdConfiguration();
			return true;
		}
	}
	else if (_params.count() == 3) {
		if (_params[1] == "data") {
			QDir d(_params.back());
			if (!d.exists()) {
				setColor(255, 0, 0);
				print("The specified directory does not exist\n");
				return false;
			}
			else {
				m_dataPath = _params.back();
				return true;
			}
		}
	}

	setColor(255, 0, 0);
	print("Invalid arguments for \"" + key() + "\"\n");
	return false;
}

// ######################################################################################################

// Command handling

void aci::Settings::cmdConfiguration(void) {
	showDelimiterLine();
	print("###  ");
	print(key());
	print(" configuration  ###\n\n");

	setColor(255, 150, 50);
	print("\tdata path                       ");
	setColor(255, 255, 255);
	print(m_dataPath + "\n");
}

// ######################################################################################################

// Protected functions

void aci::Settings::showCommandInfo(void) {
	setColor(255, 150, 50);
	print("\tcfg                       ");
	setColor(255, 255, 255);
	print("Will show the current settings configuration\n");

	setColor(255, 150, 50);
	print("\tdata <path>               ");
	setColor(255, 255, 255);
	print("Will set the data path to the path provided\n");
}

// ######################################################################################################

// Private functions

aci::Settings::Settings() {
	m_dataPath = uiAPI::settings::getString("DataPath", QDir::currentPath());
}

aci::Settings::~Settings() {
	uiAPI::settings::setString("DataPath", m_dataPath);
}