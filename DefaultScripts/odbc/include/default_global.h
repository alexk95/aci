#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ODBC_LIB)
#  define ODBC_LIB_EXPORT Q_DECL_EXPORT
# else
#  define ODBC_LIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define ODBC_LIB_EXPORT
#endif
