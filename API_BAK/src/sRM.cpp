#include "sRM.h"

sRM::sRM() {}

sRM::~sRM() {}

bool sRM::handle(const QString& _command, const QStringList& _params) {
	return false;
}

// #####################################################################################################

// Protected functions

void sRM::showCommandInfo(void) {
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

void sRM::run(QStringList& _dataToRemove, bool _dirs, bool _subdirs) {

}