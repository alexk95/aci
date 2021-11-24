#pragma once

#include <aci/aciSharedDataTypes.h>
#include <aci/AbstractOSHandler.h>

namespace aci {
	class ACI_API_EXPORT OS {
	public:
		static OS * instance(void);
		static void clearInstance(void);

		void attachOSHandler(AbstractOSHandler * _handler);
		AbstractOSHandler * handler(void) { return m_handler; }

	private:

		AbstractOSHandler *		m_handler;

	};
}