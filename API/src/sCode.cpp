#include "sCode.h"
#include <qstringlist.h>
#include <qfile.h>

sCode::sCode() {}

sCode::~sCode() {}

bool sCode::handle(const QString& _command, const QStringList& _params) {
	bool foundI{ false };
	bool foundO{ false };
	if (_params.count() < 2) {
		print("Invalid number of arguments\n\tTry: code ?\n");
		return false;
	}
	for (int i = 1; i < _params.count(); i++) {
		
	}
	return true;
}

void sCode::cmdImport(const QString& _filename) {
	if (_filename.isEmpty()) { print("[ERROR] [code]: [IMPORT]: Filename is empty\n"); }
	if (!QFile::exists(_filename)) { print("[ERROR] [code]: [IMPORT]: File not found\n"); return; }
	QFile reader(_filename);
	if (!reader.open(QIODevice::ReadOnly)) { print("[ERROR] [code]: [IMPORT]: Failed to open file\n"); return; }
}

void sCode::cmdOutput(const QString& _filename) {

}

void sCode::cmdConvert(void) {

}

void sCode::cmdShow(void) {

}

// #####################################################################################################

// Protected functions

void sCode::showCommandInfo(void) {
	print("-i <file> | import\n-o <file> | output\nrun\nshow");
}