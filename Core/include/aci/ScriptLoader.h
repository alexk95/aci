#pragma once

#include <aci/aciSharedDataTypes.h>

#include <string>
#include <list>

namespace aci {

	class ExternalDllScript;
	class InterpreterCore;
	
	class ScriptLoader {
	public:
		ScriptLoader(InterpreterCore * _core);
		virtual ~ScriptLoader();

		void loadDll(const std::wstring& _path, const std::wstring& _filename);
		void loadDllsFromDirectory(const std::wstring& _directoryPath);

	private:
		
		std::list<ExternalDllScript *>	m_externalDlls;
		InterpreterCore *				m_core;

		ScriptLoader() = delete;
		ScriptLoader(ScriptLoader&) = delete;
		ScriptLoader& operator = (ScriptLoader&) = delete;
	};

}
