#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SOURCE_LIB)
#  define SOURCE_EXPORT Q_DECL_EXPORT
# else
#  define SOURCE_EXPORT Q_DECL_IMPORT
# endif
#else
# define SOURCE_EXPORT
#endif
