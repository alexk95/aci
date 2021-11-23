#pragma once

#include "aciAPIsharedTypes.h"
#include <qstring.h>

namespace aci {

	class ACI_API_EXPORT aFile {
	public:
		aFile(const QString& _filename, const QString& _fullPath);
		aFile(const aFile& _other);
		virtual ~aFile();
		aFile& operator = (const aFile& _other);

		// ##############################################################

		// Getter

		const QString& name(void) const { return m_name; }
		const QString& fullPath(void) const { return m_fullpath; }
		bool exists(void) const;

	private:
		QString		m_name;
		QString		m_fullpath;

		aFile() = delete;
	};
}