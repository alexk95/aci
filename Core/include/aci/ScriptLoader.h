#pragma once

#include <aci/aciSharedDataTypes.h>

#include <string>

namespace aci {

	class ACI_API_EXPORT InterpreterCore;

	class ScriptLoader {
	public:
		ScriptLoader(InterpreterCore * _core);
		virtual ~ScriptLoader();

		void loadDll(const std::wstring& _path, const std::wstring& _filename);
		void loadDllsFromDirectory(const std::wstring& _directoryPath);

	private:
		InterpreterCore *		m_core;

		ScriptLoader() = delete;
		ScriptLoader(ScriptLoader&) = delete;
		ScriptLoader& operator = (ScriptLoader&) = delete;
	};

}
