#include "rm.h"

rm::rm() {}

rm::~rm() {}

bool rm::handle(const std::wstring& _command, const std::list<std::wstring>& _params) {
	bool dirs{ false };
	bool subdirs{ false };
	bool files{ false };
	bool first{ true };
	bool flags{ true };
	QStringList data;
	for (auto param : _params) {
		if (param.find(L"-") != std::wstring::npos && flags) {
			if (first) {
				files = false;
				first = false;
			}
			if (param == L"-r") {
				subdirs = true;
			}
			else if (param == L"-d") {
				dirs = true;
			}
			else if (param == L"-f") {
				files = true;
			}
			else {
				setColor(255, 150, 50);
				print(L"[rm]: Unknown parameter: " + param + L"\n");
				return false;
			}
		}
		else {
			flags = false;
			data.push_back(QString::fromStdWString(param));
		}
	}
	
	return run(data, dirs, files, subdirs);
}

// #####################################################################################################

// Protected functions

void rm::showCommandInfo(void) {
	print("Syntax:\n\trm <options> <data>\n");
	print("\tOptions:\n");

	setColor(255, 150, 50);
	print("\t-r               ");
	setColor(255, 255, 255);
	print("Recursive\n");

	setColor(255, 150, 50);
	print("\t-d                 ");
	setColor(255, 255, 255);
	print("Clear directories\n");

	setColor(255, 150, 50);
	print("\t-f                 ");
	setColor(255, 255, 255);
	print("Clear files\n");
}

// #####################################################################################################

// Private functions

void rm::clearFiles(aci::aDir& _dir, bool _subs) {

}

void rm::clearDirs(aci::aDir& _dir, bool _subs) {

}

bool rm::run(QStringList& _dataToRemove, bool _dirs, bool _files, bool _subdirs) {

	return false;
}