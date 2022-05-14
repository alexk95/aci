#pragma once

#pragma warning(disable: 4251)

#include <aci/aciSharedDataTypes.h>

#include <string>
#include <list>

namespace aci {

	class ExternalDllScript;
	class InterpreterCore;
	
	class ACI_API_EXPORT ScriptLoader {
	public:
		ScriptLoader(InterpreterCore * _core);
		virtual ~ScriptLoader();

		void loadDll(const std::wstring& _path, const std::wstring& _filename);
		void loadDllsFromDirectory(const std::wstring& _directoryPath);

		void unloadScripts(void);

	private:
		
		std::list<ExternalDllScript *>	m_externalDlls;
		InterpreterCore *				m_core;

		ScriptLoader() = delete;
		ScriptLoader(ScriptLoader&) = delete;
		ScriptLoader& operator = (ScriptLoader&) = delete;
	};

}
