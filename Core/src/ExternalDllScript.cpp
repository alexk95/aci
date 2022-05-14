#include <aci/ExternalDllScript.h>
#include <aci/InterpreterObject.h>

aci::ExternalDllScript::ExternalDllScript(HINSTANCE _libraryInstance, DLLGenerateObjectsFunctionType _functionRef, InterpreterObject * _interpreterObject, const std::wstring& _filePath)
	: m_libraryInstance(_libraryInstance), m_functionRef(_functionRef), m_interptreterObject(_interpreterObject), m_filePath(_filePath) {}

aci::ExternalDllScript::~ExternalDllScript() {}