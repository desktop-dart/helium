#ifndef HELIUM_GLOBAL_H
#define HELIUM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HELIUM_LIBRARY)
#  define HELIUMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define HELIUMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HELIUM_GLOBAL_H
