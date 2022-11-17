#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CLEANTEXT_LIB)
#  define CLEANTEXT_API_EXPORT Q_DECL_EXPORT
# else
#  define CLEANTEXT_API_EXPORT Q_DECL_IMPORT
# endif
#else
# define MERGE_EXPORT
#endif
