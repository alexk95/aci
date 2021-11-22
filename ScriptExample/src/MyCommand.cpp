#include "MyCommand.h"

MyCommand::MyCommand() {

}

MyCommand::~MyCommand() {

}

QString MyCommand::key(void) const {
	return "test";
}

bool MyCommand::handle(const QString& _command, const QStringList& _params) {
	if (_params.count() == 2) {
		print("two\n");
	}
	else {
		print("some else\n");
	}
	return false;
}

void MyCommand::showCommandInfo(void) {

}
