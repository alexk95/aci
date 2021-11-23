#pragma once

#include "aciAPIsharedTypes.h"

#include <qstring.h>
#include <list>

namespace aci {
	class aFile;

	class ACI_API_EXPORT aDir {
	public:

		aDir(const QString& _name, const QString& _fullPath);
		aDir(const aDir& _other);
		virtual ~aDir();
		aDir& operator = (const aDir& _other);

		// ##########################################################

		// IO operations

		void scanAll(void);
		void scanDirectories(bool _scanSubdirectories = true);
		void scanFiles(bool _scanSubdirectories = true);
		void filterDirectoriesWithWhitelist(const std::list<QString>& _list);
		void filterDirectoriesWithBlacklist(const std::list<QString>& _list);
		void filterFilesWithWhitelist(const std::list<QString>& _list);
		void filterFilesWithBlacklist(const std::list<QString>& _list);

		// ##########################################################

		// Setter
		void clearSubdirectories(void);
		void clearFiles(void);

		// ##########################################################

		// Getter

		inline const QString& name(void) const { return m_name; }
		inline const QString& fullPath(void) const { return m_fullPath; }
		inline const std::list<aDir *>& subdirectories(void) const { return m_subDirectories; }
		std::list<QString> subdirectoryNameList(void) const;
		inline const std::list<aFile *>& files(void) const { return m_files; }
		std::list<QString> fileNameList(void) const;
		bool exists(void) const;
		size_t subDirCount(void);
		size_t fileCount(void);

		// ##############################################################

		// Static functions

		static QString lastFolder(const QString& _path);

	private:
		QString				m_name;
		QString				m_fullPath;
		std::list<aDir *>	m_subDirectories;
		std::list<aFile *>	m_files;

		aDir() = delete;
	};
}