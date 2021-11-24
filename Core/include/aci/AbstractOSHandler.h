#pragma once

#include <aci/aciSharedDataTypes.h>

#include <string>
#include <list>

namespace aci {
	class ACI_API_EXPORT AbstractOSHandler {
	public:
		AbstractOSHandler() {}
		virtual ~AbstractOSHandler() {}

		virtual bool fileExists(const std::wstring& _path) = 0;
		virtual bool deleteFile(const std::wstring& _path) = 0;
		virtual bool readLinesFromFile(std::list<std::wstring>& _data, const std::wstring& _path) = 0;
		virtual bool writeLinesToFile(const std::list<std::wstring>& _data, const std::wstring& _path) = 0;

		virtual bool directoryExists(const std::wstring& _path) = 0;
		virtual std::list<std::wstring> filesInDirectory(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) = 0;
		virtual std::list<std::wstring> subdirectories(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) = 0;
		virtual std::wstring currentDirectory(void) = 0;
		virtual std::wstring scriptDirectory(void) = 0;

		virtual std::wstring getSettingsValue(const std::string& _key, const std::wstring& _defaultValue = std::wstring()) = 0;
		virtual void setSettingsValue(const std::string& _key, const std::wstring& _value) = 0;
	};
}