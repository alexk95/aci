#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DEFAULT_LIB)
#  define DEFAULT_LIB_EXPORT Q_DECL_EXPORT
# else
#  define DEFAULT_LIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define DEFAULT_LIB_EXPORT
#endif
