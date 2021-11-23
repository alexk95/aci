// aci header
#include <aci/Settings.h>
#include <aci/aDir.h>
#include <aci/OS.h>

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

bool aci::Settings::handle(const std::wstring& _command, const std::list<std::wstring>& _params) {
	if (_params.size() == 2) {
		if (_params.back() == L"cfg") {
			cmdConfiguration();
			return true;
		}
	}
	else if (_params.size() == 3) {
		auto it = _params.begin();
		it++;
		if (*it == L"data") {
			aDir d(L"", _params.back());
			if (!d.exists()) {
				setColor(255, 0, 0);
				print(L"The specified directory does not exist\n");
				return false;
			}
			else {
				m_dataPath = _params.back();
				return true;
			}
		}
	}

	setColor(255, 0, 0);
	print(L"Invalid arguments for \"" + key() + L"\"\n");
	return false;
}

// ######################################################################################################

// Command handling

void aci::Settings::cmdConfiguration(void) {
	showDelimiterLine();
	print(L"###  ");
	print(key());
	print(L" configuration  ###\n\n");

	setColor(255, 150, 50);
	print(L"\tdata path                       ");
	setColor(255, 255, 255);
	print(m_dataPath + L"\n");
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
	AbstractOSHandler * os = OS::instance()->handler();
	m_dataPath = os->getSettingsValue("DataPath", os->currentDirectory());
}

aci::Settings::~Settings() {
	AbstractOSHandler * os = OS::instance()->handler();
	os->setSettingsValue("DataPath", m_dataPath);
}