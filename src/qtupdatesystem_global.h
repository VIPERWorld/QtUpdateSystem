#ifndef QTUPDATESYSTEM_GLOBAL_H
#define QTUPDATESYSTEM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTUPDATESYSTEM_LIBRARY)
#  define QTUPDATESYSTEMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTUPDATESYSTEMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTUPDATESYSTEM_GLOBAL_H
