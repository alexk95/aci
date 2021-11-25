#include <aci/ScriptLoader.h>
#include <aci/InterpreterCore.h>
#include <aci/AbstractPrinter.h>
#include <aci/ExternalDllScript.h>

aci::ScriptLoader::ScriptLoader(InterpreterCore * _core) : m_core(_core) {}

aci::ScriptLoader::~ScriptLoader() {

}

void aci::ScriptLoader::loadDll(const std::wstring& _path, const std::wstring& _filename) {
	m_core->printer()->setColor(255, 255, 255);
	m_core->printer()->print(L"Load library: " + _filename);
	HINSTANCE hGetProcIDDLL = LoadLibrary(_path.c_str());

	if (hGetProcIDDLL) {
		// Resolve function address here
		ExternalDllScript::DLLGenerateObjectsFunctionType func = (ExternalDllScript::DLLGenerateObjectsFunctionType)GetProcAddress(hGetProcIDDLL, "generateObjects");
		if (func) {
			int objCt{ 0 };
			aci::InterpreterObject ** obj = func(objCt);
			int ct{ 0 };
			
			for (int o{ 0 }; o < objCt; o++) {
				if (obj[o]) {
					ExternalDllScript * newScript = new ExternalDllScript(hGetProcIDDLL, func, obj[o]);
					m_core->addScriptObject(obj[o]);
					ct++;
				}
			}

			if (ct == 0) {
				m_core->printer()->setColor(255, 150, 50);
				m_core->printer()->print(L"  DONE");
				m_core->printer()->setColor(255, 255, 255);
				m_core->printer()->print(L" (no commands loaded)\n");
			}
			else {
				m_core->printer()->setColor(0, 255, 0);
				m_core->printer()->print(L"  SUCCESS");
				m_core->printer()->setColor(255, 255, 255);
				m_core->printer()->print(" (" + std::to_string(ct) + " commands loaded)\n");
			}
		}
		else {
			m_core->printer()->setColor(255, 0, 0);
			m_core->printer()->print(L"  FAILED (Entry point not found)\n");
		}
	}
	else {
		m_core->printer()->setColor(255, 0, 0);
		m_core->printer()->print(L"  FAILED (dll not loaded)\n");
	}
}

void aci::ScriptLoader::loadDllsFromDirectory(const std::wstring& _directoryPath) {

}