#pragma once

#ifdef ACI_EXPORTS
#define ACI_API_EXPORT __declspec(dllexport)
#else
#define ACI_API_EXPORT __declspec(dllimport)
#endif