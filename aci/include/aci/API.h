#pragma once

#include <aci/aciSharedDataTypes.h>

namespace aci {

	class InterpreterCore;
	class AbstractPrinter;
	class AbstractInterpreterNotifier;

	namespace API {

		ACI_API_EXPORT bool initialize(AbstractPrinter * _printer, AbstractInterpreterNotifier * _notifier);

		ACI_API_EXPORT void cleanUp(void) noexcept;

		ACI_API_EXPORT InterpreterCore * core(void);

	}

}
