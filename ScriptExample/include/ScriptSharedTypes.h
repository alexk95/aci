#pragma once

#ifdef SCRIPT_EXPORT
#define SCRIPT_API_EXPORT __declspec(dllexport)
#else
#define SCRIPT_API_EXPORT __declspec(dllimport)
#endif // SCRIPT_EXPORT
