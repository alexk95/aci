#pragma once

#include <aci/aciSharedDataTypes.h>

#include <string>
#include <list>

namespace aci {

	ACI_API_EXPORT std::list<std::wstring> splitString(const std::wstring& _str, const std::wstring& _splitBy);

}