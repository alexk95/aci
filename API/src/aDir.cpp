#include <aDir.h>
#include <aFile.h>

#include <qdir.h>
#include <qdiriterator.h>

using namespace aci;

aDir::aDir(const QString& _name, const QString& _fullPath) 
	: m_name(_name), m_fullPath(_fullPath) {}

aDir::aDir(const aDir& _other) 
	: m_name(_other.m_name), m_fullPath(_other.m_fullPath)
{
	for (auto sd : _other.m_subDirectories) {
		m_subDirectories.push_back(sd);
	}
	for (auto f : _other.m_files) {
		m_files.push_back(f);
	}
}

aDir::~aDir() {
	clearSubdirectories();
	clearFiles();
}

aDir& aDir::operator = (const aDir& _other) {
	m_name = _other.m_name;
	m_fullPath = _other.m_fullPath;

	clearSubdirectories();
	for (auto sd : _other.m_subDirectories) {
		m_subDirectories.push_back(sd);
	}
	clearFiles();
	for (auto f : _other.m_files) {
		m_files.push_back(f);
	}

	return *this;
}

// ##########################################################

// IO operations

void aDir::scanAll(void) {
	clearSubdirectories();

	QDir directory(m_fullPath + "/");
	directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	QStringList fileList = directory.entryList();
	for (int i = 0; i < fileList.count(); i++)
	{
		aDir * dir = new aDir(fileList[i], m_fullPath + "/" + fileList[i]);
		dir->scanAll();
		m_subDirectories.push_back(dir);
	}
	scanFiles(true);
}

void aDir::scanDirectories(bool _scanSubdirectories) {
	clearSubdirectories();

	QDir directory(m_fullPath + "/");
	directory.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	if (_scanSubdirectories) {
		QStringList fileList = directory.entryList();
		for (int i = 0; i < fileList.count(); i++)
		{
			m_subDirectories.push_back(new aDir(fileList[i], m_fullPath + "/" + fileList[i]));
		}
	}
}

void aDir::scanFiles(bool _scanSubdirectories) {
	clearFiles();

	QDir directory(m_fullPath + "/");
	directory.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	QStringList fileList = directory.entryList();
	for (int i = 0; i < fileList.count(); i++)
	{
		m_files.push_back(new aFile(fileList[i], m_fullPath + "/" + fileList[i]));
	}
}

void aDir::filterDirectoriesWithWhitelist(const std::list<QString>& _list) {
	bool erased{ true };

	// Check subdirectories
	while (erased) {
		erased = false;
		for (auto it = m_subDirectories.begin(); it != m_subDirectories.end(); it++) {
			bool ok{ false };
			for (auto filter : _list) {
				if (it._Ptr->_Myval->name().contains(filter) || it._Ptr->_Myval->fullPath().contains(filter)) { ok = true; break; }
			}
			if (!ok) {
				m_subDirectories.erase(it);
				erased = true;
				break;
			}
		}
	}

	// Check subs of subs
	for (auto sub : m_subDirectories) {
		sub->filterDirectoriesWithWhitelist(_list);
	}
}

void aDir::filterDirectoriesWithBlacklist(const std::list<QString>& _list) {
	bool erased{ true };

	// Check subdirectories
	while (erased) {
		erased = false;
		for (auto it = m_subDirectories.begin(); it != m_subDirectories.end(); it++) {
			for (auto filter : _list) {
				if (it._Ptr->_Myval->name().contains(filter) || it._Ptr->_Myval->fullPath().contains(filter)) {
					m_subDirectories.erase(it);
					filterDirectoriesWithBlacklist(_list);
					return;
				}
			}

			if (erased) { break; }
		}
	}
	
	// Check subs of subs
	for (auto sub : m_subDirectories) {
		sub->filterDirectoriesWithBlacklist(_list);
	}
}

void aDir::filterFilesWithWhitelist(const std::list<QString>& _list) {
	bool erased{ true };

	// Check files
	erased = true;
	while (erased) {
		erased = false;
		for (auto it = m_files.begin(); it != m_files.end(); it++) {
			bool ok{ false };
			for (auto filter : _list) {
				if (it._Ptr->_Myval->name().contains(filter) || it._Ptr->_Myval->fullPath().contains(filter)) { ok = true; break; }
			}
			if (!ok) {
				m_files.erase(it);
				erased = true;
				break;
			}
		}
	}

	// Check subs of subs
	for (auto sub : m_subDirectories) {
		sub->filterFilesWithWhitelist(_list);
	}
}

void aDir::filterFilesWithBlacklist(const std::list<QString>& _list) {
	bool erased{ true };

	// Check files
	erased = true;
	while (erased) {
		erased = false;
		for (auto it = m_files.begin(); it != m_files.end(); it++) {
			for (auto filter : _list) {
				if (it._Ptr->_Myval->name().contains(filter) || it._Ptr->_Myval->fullPath().contains(filter)) {
					m_files.erase(it);
					filterFilesWithBlacklist(_list);
					return;
				}
			}

			if (erased) { break; }
		}
	}

	// Check subs of subs
	for (auto sub : m_subDirectories) {
		sub->filterFilesWithBlacklist(_list);
	}
}

// ##########################################################

// Setter

void aDir::clearSubdirectories(void) {
	for (auto d : m_subDirectories) {
		delete d;
	}
	m_subDirectories.clear();
}

void aDir::clearFiles(void) {
	for (auto f : m_files) {
		delete f;
	}
	m_files.clear();
}

// ##############################################################

// Getter

std::list<QString> aDir::subdirectoryNameList(void) const {
	std::list<QString> ret;
	for (auto sub : m_subDirectories) {
		ret.push_back(sub->name());
		for (auto subsub : sub->subdirectoryNameList()) {
			ret.push_back(sub->name() + "/" + subsub);
		}
	}
	return ret;
}

std::list<QString> aDir::fileNameList(void) const {
	std::list<QString> ret;
	for (auto f : m_files) {
		ret.push_back(f->name());
	}
	for (auto sub : m_subDirectories) {
		for (auto subFile : sub->fileNameList()) {
			ret.push_back(sub->name() + "/" + subFile);
		}
	}
	return ret;
}

bool aDir::exists(void) const {
	QDir dir(fullPath());
	return dir.exists();
}

size_t aDir::subDirCount(void) {
	size_t ret = m_subDirectories.size();
	for (auto sub : m_subDirectories) {
		ret += sub->subDirCount();
	}
	return ret;
}

size_t aDir::fileCount(void) {
	size_t ret = m_files.size();
	for (auto sub : m_subDirectories) {
		ret += sub->fileCount();
	}
	return ret;
}

// ##############################################################

// Static functions

QString aDir::lastFolder(const QString& _path) {
	QString str = _path;
	str.replace("\\", "/");
	QStringList lst = str.split("/");
	return lst.back();
}
