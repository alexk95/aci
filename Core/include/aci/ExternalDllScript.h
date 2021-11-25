#pragma once

#include <Windows.h>

namespace aci {

	class InterpreterObject;

	class ExternalDllScript {
	public:
		typedef InterpreterObject ** (__stdcall *DLLGenerateObjectsFunctionType)(int& _count);

		ExternalDllScript(HINSTANCE _libraryInstance, DLLGenerateObjectsFunctionType _functionRef, InterpreterObject * _interpreterObject);
		virtual ~ExternalDllScript();

		HINSTANCE libraryInstance(void) { return m_libraryInstance; }
		DLLGenerateObjectsFunctionType generateFunctionRef(void) { return m_functionRef; }
		InterpreterObject * interptreterObject(void) { return m_interptreterObject; }

	private:

		HINSTANCE							m_libraryInstance;
		DLLGenerateObjectsFunctionType		m_functionRef;
		InterpreterObject *					m_interptreterObject;

		ExternalDllScript() = delete;
		ExternalDllScript(ExternalDllScript&) = delete;
		ExternalDllScript& operator = (ExternalDllScript&) = delete;
	};
}
