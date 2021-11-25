#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MERGE_LIB)
#  define MERGE_EXPORT Q_DECL_EXPORT
# else
#  define MERGE_EXPORT Q_DECL_IMPORT
# endif
#else
# define MERGE_EXPORT
#endif
