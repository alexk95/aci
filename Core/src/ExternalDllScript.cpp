#include <aci/ExternalDllScript.h>

aci::ExternalDllScript::ExternalDllScript(HINSTANCE _libraryInstance, DLLGenerateObjectsFunctionType _functionRef, InterpreterObject * _interpreterObject) 
	: m_libraryInstance(_libraryInstance), m_functionRef(_functionRef), m_interptreterObject(m_interptreterObject) {}
aci::ExternalDllScript::~ExternalDllScript() {
	
}