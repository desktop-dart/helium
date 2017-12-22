#ifndef HELIUM_H
#define HELIUM_H

#include "helium_global.h"

#include <QApplication>
#include <QtWebEngineWidgets/QWebEngineView>

extern "C"
HELIUMSHARED_EXPORT
void appInit(void);

extern "C"
HELIUMSHARED_EXPORT
void processEvents(void);

extern "C"
HELIUMSHARED_EXPORT
QWebEngineView& createWindow(void);

extern "C"
HELIUMSHARED_EXPORT
void setVisibilityWindow(QWebEngineView& window, bool visible);

extern "C"
HELIUMSHARED_EXPORT
void showWindow(QWebEngineView& window);

extern "C"
HELIUMSHARED_EXPORT
void hideWindow(QWebEngineView& window);

extern "C"
HELIUMSHARED_EXPORT
void resizeWindow(QWebEngineView& window, int w, int h);

extern "C"
HELIUMSHARED_EXPORT
void setUrlWindow(QWebEngineView& window, char const * url);

#endif // HELIUM_H
