#include "odbc.h"

#include <qvariant.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>

using namespace aci;

odbc::odbc() : m_configuration(NONE), m_columns(1) {}

odbc::~odbc() {}

bool odbc::handle(const std::wstring& _command, const std::vector<std::wstring>& _params) {
	if (_params.size() == 2) {
		if (_params[1] == L"cfg") {
			showConfiguration();
			return true;
		}
		else if (_params[1] == L"excel") {
			m_configuration = EXCEL;
			m_driver = "Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)";
			return true;
		}
		else if (_params[1] == L"debug") {
			m_configuration = EXCEL;
			m_driver = "Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)";
			m_file = "C:\\Users\\alexk\\Desktop\\Videos\\Test.xlsx";
			m_columns = 2;
			return true;
		}
	}
	else if (_params.size() == 3) {
		if (_params[1] == L"driver") {
			m_driver = QString::fromStdWString(_params[2]);
			m_configuration = NONE;
			return true;
		}
		else if (_params[1] == L"file") {
			m_file = QString::fromStdWString(_params[2]);
			return true;
		}
		else if (_params[1] == L"run" || _params[1] == L"exec") {
			return executeQuery(QString::fromStdWString(_params[2]));
		}
		else if (_params[1] == L"columns") {
			int columns = QString::fromStdWString(_params[2]).toInt();
			if (columns < 1) {
				printPrefix();
				setColor(Color::RED);
				print("Invalid ammont of columns provided (must be greater than 0)\n");
				return false;
			}
			m_columns = columns;
			return true;
		}
	}
	
	printPrefix();
	setColor(Color::RED);
	print(L"Invalid arguments for \"odbc\". Try \"?odbc\" for help\n");
	return false;
}

// #####################################################################################################

// Protected functions

void odbc::showCommandInfo(void) {
	printPrefix();
	print("? ODBC is used to execute sql queries\n");

	printPrefix();
	print("? The script contains default configurations for the given databases:\n");
	
	printPrefix();
	print("?     -Microsoft excel\n\n");

	printPrefix();
	print("  cfg               \tWill display the current configuration\n");

	printPrefix();
	setColor(Color::YELLOW);
	print("  run <query>       \tWill execute the query and display the results\n");

	printPrefix();
	print("  excel             \tWill configure the driver for a Microsoft Excel connection (Filename must be set before running a query)\n");

	printPrefix();
	setColor(Color::YELLOW);
	print("  driver <string>    \tWill set the driver configuration string\n");

	printPrefix();
	print("  file <string>      \tWill set the filename\n");

	printPrefix();
	setColor(Color::YELLOW);
	print("  columns <integer>  \tWill set the number of columns in the query result\n");
}

// #####################################################################################################

// Private functions

void odbc::printPrefix(void) {
	setColor(Color::WHITE);
	print("[odbc] ");
}

void odbc::showConfiguration(void) {
	printDelimiterLine();
	printPrefix();
	print("Default configuration:\t\t");
	switch (m_configuration)
	{
	case odbc::NONE: print("none\n"); break;
	case odbc::EXCEL: print("excel\n"); break;
	default: print("<UNKNOWN>\n"); break;
	}

	printPrefix();
	setColor(Color::YELLOW);
	print("Driver:               \t\t");
	if (m_driver.isEmpty()) print("<empty>\n");
	else print(m_driver.toStdString() + "\n");

	printPrefix();
	print("File:                 \t\t");
	if (m_file.isEmpty()) print("<empty>\n");
	else print(m_file.toStdString() + "\n");

	printPrefix();
	setColor(Color::YELLOW);
	print("Columns               \t\t");
	print(std::to_string(m_columns));
}

bool odbc::executeQuery(const QString& _query) {
	// Check excel configuration
	if (m_configuration == EXCEL) {
		if (m_file.isEmpty()) {
			printPrefix();
			setColor(Color::RED);
			print("No file provided for excel configuration\n");
			return false;
		}
		else if (!m_file.endsWith(".xls") && !m_file.endsWith(".xlsx") && !m_file.endsWith(".xlsm") && !m_file.endsWith(".xlsb")) {
			printPrefix();
			setColor(Color::RED);
			print("Invalid file type for excel configuration (See driver string for valid file types)\n");
			return false;
		}
	};
	// Check required data
	if (m_driver.isEmpty()) {
		printPrefix();
		setColor(Color::RED);
		print("No driver configuration string provided\n");
		return false;
	}

	QString connectionString("DRIVER={");
	connectionString.append(m_driver).append("}");

	if (!m_file.isEmpty()) {
		connectionString.append(";DBQ=").append(m_file);
	}

	printPrefix();
	print("Trying to open connection with connection string: " + connectionString.toStdString() + "\n");

	// Open connection
	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "ACI_SCRIPT_ODBC_CONNECTION");
	db.setDatabaseName(connectionString);

	if (!db.open()) {
		printPrefix();
		setColor(Color::RED);
		print("Failed to open connection to ODBC database\n");
		return false;
	}

	try {
		printPrefix();
		print("Executing query\n");

		// Execute query
		QSqlQuery query(_query, db);
		// Display query results
		int row{ 0 };
		do {
			for (int col = 0; col < m_columns; col++) {
				printPrefix();
				setColor(Color::YELLOW);
				print("[" + std::to_string(row) + "|" + std::to_string(col) + "]: ");
				setColor(Color::GREEN);
				print(query.value(col).toString().toStdWString() + L"\n");
			}
			print("\n");
			row++;
		} while (query.next());

	}
	catch (const std::exception& e) {
		printPrefix();
		setColor(Color::RED);
		print("Failed to execute query: " + std::string(e.what()) + "\n");
		// Close connection
		db.close();
		QSqlDatabase::removeDatabase("ACI_SCRIPT_ODBC_CONNECTION");
		return false;
	}
	catch (...) {
		printPrefix();
		setColor(Color::RED);
		print("Failed to execute query: Unknown error\n");
		// Close connection
		db.close();
		QSqlDatabase::removeDatabase("ACI_SCRIPT_ODBC_CONNECTION");
		return false;
	}

	printPrefix();
	print("Closing connection\n");

	// Close connection
	db.close();
	QSqlDatabase::removeDatabase("ACI_SCRIPT_ODBC_CONNECTION");

	return true;
}