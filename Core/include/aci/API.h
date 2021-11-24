#pragma once

#include <aci/aciSharedDataTypes.h>

#include <string>

namespace aci {

	class InterpreterCore;
	class AbstractPrinter;
	class AbstractOSHandler;
	class AbstractInterpreterNotifier;

	namespace API {

		ACI_API_EXPORT bool initialize(AbstractPrinter * _printer, AbstractInterpreterNotifier * _notifier, AbstractOSHandler * _osHandler);

		ACI_API_EXPORT void cleanUp(void) noexcept;

		ACI_API_EXPORT InterpreterCore * core(void);

		ACI_API_EXPORT std::wstring currentPath(void);

		ACI_API_EXPORT void setCurrentPath(const std::wstring& _path);
	}

}
