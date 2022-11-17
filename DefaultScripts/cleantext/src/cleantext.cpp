#include "cleantext.h"
#include <aci/aFile.h>
#include <aci/Color.h>

#include <list>
#include <fstream>
#include <sstream>

using namespace aci;

cleantext::cleantext(void)
	: m_prefix(L"[cleantext] ")
{}

cleantext::~cleantext() {

}

bool cleantext::handle(const std::wstring& _command, const std::vector<std::wstring>& _params) {
	if (_command.length() > 2) {
		enum currentOption {
			none,
			prefixRemove,
			prefixAdd,
			suffixRemove,
			suffixAdd,
		};
		currentOption opt = none;
		std::wstring filename = _params[1];
		std::list<std::wstring> preRemove;
		std::wstring pre;
		std::list<std::wstring> sufRemove;
		std::wstring suf;
		bool clearEmptyLines = false;
		bool addQuotation = false;
		bool toLowerCase = false;
		bool toUpperCase = false;
		bool linesUnique = false;

		for (size_t i = 2; i < _params.size(); i++) {
			if (_params[i] == L"-pre") opt = prefixRemove;
			else if (_params[i] == L"+pre") opt = prefixAdd;
			else if (_params[i] == L"-suf") opt = suffixRemove;
			else if (_params[i] == L"+suf") opt = suffixAdd;
			else if (_params[i] == L"-noempty") clearEmptyLines = true;
			else if (_params[i] == L"-addquot") addQuotation = true;
			else if (_params[i] == L"-tolower") toLowerCase = true;
			else if (_params[i] == L"-toupper") toUpperCase = true;
			else if (_params[i] == L"-unique") linesUnique = true;
			else if (opt == none) {
				setColor(Color::RED);
				print(m_prefix + L"Invalid option \"" + _params[i] + L"\" for \"" + key() + L"\"\n");
				return false;
			}
			else {
				switch (opt)
				{
					break;
				case prefixRemove:
					if (!_params[i].empty()) {
						if (_params[i] == L"\\t") preRemove.push_back(L"\t");
						else preRemove.push_back(_params[i]);
					}
					break;
				case prefixAdd:
					pre = _params[i];
					opt = none;
					break;
				case suffixRemove:
					if (!_params[i].empty()) {
						if (_params[i] == L"\\t") sufRemove.push_back(L"\t");
						else sufRemove.push_back(_params[i]);
					}
					break;
				case suffixAdd:
					suf = _params[i];
					opt = none;
					break;
				default:
					setColor(Color::RED);
					print(m_prefix + L"Not implemented option for \"" + key() + L"\"\n");
					return false;
					break;
				}
			}
		}

		return run(filename, preRemove, pre, sufRemove, suf, clearEmptyLines, addQuotation, toLowerCase, toUpperCase, linesUnique);
	}

	setColor(Color::RED);
	print(m_prefix + L"Invalid arguments for \"" + key() + L"\"\n");
	return false;
}

bool cleantext::run(const std::wstring& _filename, const std::list<std::wstring>& _prefixRemove, const std::wstring& _newPrefix, const std::list<std::wstring>& _suffixRemove, const std::wstring& _newSuffix, bool _removeEmptyLines, bool _addQuotation, bool _toLower, bool _toUpper, bool _linesUnique) {
	if (_prefixRemove.empty() && _suffixRemove.empty() &&
		_newPrefix.empty() && _newSuffix.empty() &&
		!_removeEmptyLines && !_addQuotation && !_toLower && !_toUpper)
	{
		setColor(Color::ORANGE);
		print(m_prefix + L"Nothing to do. Use \"?" + key() + L"\" for help");
		return false;
	}

	std::list<std::wstring> result;
	std::list<std::wstring> tmp;

	// Read file
	{
		std::wifstream reader;
		reader.open(_filename);
		if (!reader.is_open()) {
			setColor(Color::RED);
			print(m_prefix + L"Failed to open file for reading\n");
			return false;
		}

		std::wstring line;
		while (std::getline(reader, line)) {
			tmp.push_back(line);
		}
		reader.close();
	}

	// Clean file
	for (auto l : tmp) {
		std::wstring tidyLine = l;
		bool wasCleaned = true;
		while (wasCleaned) {
			wasCleaned = false;
			// Prefix
			for (auto pre : _prefixRemove) {
				if (!tidyLine.empty()) {
					if (tidyLine.find(pre) == 0) {
						wasCleaned = true;
						tidyLine = tidyLine.substr(pre.length());
						break;
					}
				}
			}
			// Suffix
			for (auto suf : _suffixRemove) {
				if (!tidyLine.empty()) {
					if (tidyLine.rfind(suf) == (tidyLine.length() - suf.length())) {
						wasCleaned = true;
						tidyLine = tidyLine.substr(0, tidyLine.length() - suf.length());
						break;
					}
				}
			}
		}

		if (tidyLine.size() == 0 && _removeEmptyLines) continue;
		
		if (_addQuotation) tidyLine = L"\"" + tidyLine + L"\"";

		if (_toLower) std::transform(tidyLine.begin(), tidyLine.end(), tidyLine.begin(), ::tolower);
		if (_toUpper) std::transform(tidyLine.begin(), tidyLine.end(), tidyLine.begin(), ::toupper);

		if (!_newPrefix.empty()) tidyLine = _newPrefix + tidyLine;
		if (!_newSuffix.empty()) tidyLine.append(_newSuffix);

		bool found = false;
		if (_linesUnique) {
			for (auto currentLine : result) {
				if (currentLine == tidyLine) {
					found = true;
					break;
				}
			}
		}
		if (!found) result.push_back(tidyLine);	
	}

	// Display results
	if (!result.empty()) {
		setColor(Color::WHITE);
		print("\n ### FILE START ###\n");
		for (auto l : result) print(l + L"\n");
		print("\n ### FILE END ###\n");
	}
	return true;
}

// #####################################################################################################

// Protected functions

void cleantext::showCommandInfo(void) {
	setColor(Color::WHITE);
	print(m_prefix + L"? cleantext is used to clean all files from not needed characters\n\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t");
	print(L"run <options>    Run\n");
	
	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	print("-pre       Add a string that should be removed from the prefix\n");
	
	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	setColor(Color::YELLOW);
	print("+pre       Set the prefix string that should be added to every line\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	print("-suf       Add a string that should be removed from the suffix\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	setColor(Color::YELLOW);
	print("+suf       Set the suffix string that should be added to every line\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	print("-noempty   Remove all empty lines (line will be evaluated after removing suffix and prefix)\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	setColor(Color::YELLOW);
	print("-addquot   Will add quotations \" to the line before +pre/+suf but after -pre/-suf\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	print("-tolower   Will replace upper case characters with their lower case (before +pre/+suf)\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	setColor(Color::YELLOW);
	print("-toupper   Will replace lower case characters with their upper case (before +pre/+suf)\n");

	setColor(Color::WHITE);
	print(m_prefix + L"\t\t");
	print("-unique   Every line (after performing all steps) must be unique (duplicates will be removed)\n");
}
