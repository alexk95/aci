#include <aFile.h>
#include <qfile.h>

using namespace aci;

aFile::aFile(const QString& _filename, const QString& _fullPath)
	: m_name(_filename), m_fullpath(_fullPath) {}

aFile::aFile(const aFile& _other) : m_name(_other.m_name), m_fullpath(_other.m_fullpath) {}

aFile::~aFile() {}

aFile& aFile::operator = (const aFile& _other) {
	m_name = _other.m_name;
	return *this;
}

// ##############################################################

bool aFile::exists(void) const {
	return QFile::exists(m_fullpath);
}
