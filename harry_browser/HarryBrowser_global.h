#ifndef HARRYBROWSER_GLOBAL_H
#define HARRYBROWSER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HARRYBROWSER_LIBRARY)
#  define HARRYBROWSER_EXPORT Q_DECL_EXPORT
#else
#  define HARRYBROWSER_EXPORT Q_DECL_IMPORT
#endif


#endif // HARRYBROWSER_GLOBAL_H
