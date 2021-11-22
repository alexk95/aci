#pragma once

#ifdef ACI_EXPORT
#define ACI_API_EXPORT __declspec(dllexport)
#else
#define ACI_API_EXPORT __declspec(dllimport)
#endif // ACI_EXPORT

