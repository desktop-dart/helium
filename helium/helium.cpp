#include "helium.h"
#include <QApplication>
#include <QtWebEngineCore/QWebEngineUrlSchemeHandler>
#include <QtWebEngineWidgets/QWebEngineProfile>

#ifdef LIBWEBVIEW_BUILDFORDART
#include <dart_api.h>

void Print(const char * chars);
#endif

int argc = 0;

class HeliumRpcSchemeHandler : public QWebEngineUrlSchemeHandler {
public:
    explicit HeliumRpcSchemeHandler(QObject *parent) : QWebEngineUrlSchemeHandler(parent){}

    void requestStarted(QWebEngineUrlRequestJob*) {
#ifdef LIBWEBVIEW_BUILDFORDART
        Print("Request received!");
#endif
    }
};

QApplication* app;

void appInit() {
    if(app != nullptr) {
        return;
    }
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    app = new QApplication(argc, nullptr);
}

void processEvents(void) {
    app->processEvents();
}

QWebEngineView& createWindow(void) {
    auto& view = *new QWebEngineView();

/* TODO
    QWebEngineProfile& profile = *view.page()->profile();
    HeliumRpcSchemeHandler* handler = new HeliumRpcSchemeHandler(&view);
    profile.installUrlSchemeHandler(QByteArray("https"), handler);
*/

    return view;
}

void setVisibilityWindow(QWebEngineView& window, bool visible) {
    if(visible) window.show();
    else window.hide();
}

void showWindow(QWebEngineView& window) {
    window.show();
}

void hideWindow(QWebEngineView& window) {
    window.hide();
}

void resizeWindow(QWebEngineView& window, int w, int h) {
    window.resize(w, h);
}

void setUrlWindow(QWebEngineView& window, char const * url) {
    window.setUrl(QUrl(url));
}

#ifdef LIBWEBVIEW_BUILDFORDART
void Print(const char * chars) {
    Dart_EnterScope();

    auto libCore = Dart_LookupLibrary(Dart_NewStringFromCString("dart:core"));
    auto funcPrint = Dart_NewStringFromCString("print");
    Dart_Handle args[1] = {Dart_NewStringFromCString(chars)};
    Dart_Invoke(libCore, funcPrint, 1, args);

    Dart_ExitScope();
}

void HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) Dart_PropagateError(handle);
}

void Throw(const char *error) {
   Dart_Handle errStr = Dart_NewStringFromCString(error);
   HandleError(errStr);
   Dart_ThrowException(errStr);
}

namespace DartArgs {
/// Returns an argument at index \p index in arguments \p args
Dart_Handle Get(Dart_NativeArguments& args, const int index) {
   Dart_Handle arg = Dart_GetNativeArgument(args, index);
   HandleError(arg);
   return arg;
}

/// Returns an integer argument at index \p index in arguments \p args
int64_t GetInt(Dart_NativeArguments& args, int index) {
   int64_t val;
   Dart_Handle arg = Get(args, index);

   if (!Dart_IsInteger(arg))
      Throw("LibViewNativeException: Parameter is not an integer");


   HandleError(Dart_IntegerToInt64(arg, &val));
   return val;
}

/// Returns an integer argument at index \p index in arguments \p args
uint64_t GetUInt(Dart_NativeArguments& args, int index) {
   uint64_t val;
   Dart_Handle arg = Get(args, index);

   if (!Dart_IsInteger(arg))
      Throw("LibViewNativeException: Parameter is not an integer");


   HandleError(Dart_IntegerToUint64(arg, &val));
   return val;
}

/// Returns a double argument at index \p index in arguments \p args
double GetDouble(Dart_NativeArguments& args, int index) {
   double val;
   Dart_Handle arg = Get(args, index);

   if (!Dart_IsDouble(arg))
      Throw("LibViewNativeException: Parameter is not a double");


   HandleError(Dart_DoubleValue(arg, &val));
   return val;
}

bool GetBool(Dart_NativeArguments& args, int index) {
   bool val;
   Dart_Handle arg = Get(args, index);

   if (!Dart_IsBoolean(arg))
      Throw("LibViewNativeException: Parameter is not a boolean");

   HandleError(Dart_BooleanValue(arg, &val));
   return val;
}

const char* GetString(Dart_NativeArguments& args, int index) {
   const char* val;
   Dart_Handle arg = Get(args, index);

   if (!Dart_IsString(arg))
      Throw("LibViewNativeException: Parameter is not a string");

   HandleError(Dart_StringToCString(arg, &val));
   return val;
}
}

void AppInit(Dart_NativeArguments args) {
    Dart_EnterScope();
    appInit();
    Dart_SetReturnValue(args, Dart_Null());
    Dart_ExitScope();
}

void AppProcessEvents(Dart_NativeArguments args) {
    Dart_EnterScope();
    processEvents();
    Dart_SetReturnValue(args, Dart_NewInteger(argc));
    Dart_ExitScope();
}

namespace Window {
void Destroy(void*, Dart_WeakPersistentHandle, void* peer) {
    if(peer == nullptr) {
        return;
    }

    QWebEngineView* window = static_cast<QWebEngineView*> (peer);
    if (window) {
        delete window;
    }
}

void Create(Dart_NativeArguments arguments) {
    Dart_EnterScope();

    Dart_Handle windowHandle = DartArgs::Get(arguments, 0);
    QWebEngineView& window = createWindow();

    Dart_NewWeakPersistentHandle(windowHandle, (void*) &window, 0, Destroy);

    Dart_SetReturnValue(arguments, Dart_NewIntegerFromUint64((uint64_t)&window));
    Dart_ExitScope();
}

void SetVisibility(Dart_NativeArguments arguments) {
    Dart_EnterScope();

    QWebEngineView& window = *reinterpret_cast<QWebEngineView*> (DartArgs::GetUInt(arguments, 1));
    bool visibility = DartArgs::GetBool(arguments, 2);
    setVisibilityWindow(window, visibility);

    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_ExitScope();
}

void Resize(Dart_NativeArguments arguments) {
    Dart_EnterScope();

    {
        QWebEngineView& window = *reinterpret_cast<QWebEngineView*> (DartArgs::GetUInt(arguments, 1));
        int x = DartArgs::GetInt(arguments, 2);
        int y = DartArgs::GetInt(arguments, 3);
        resizeWindow(window, x, y);
    }

    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_ExitScope();
}

void SetUrl(Dart_NativeArguments arguments) {
    Dart_EnterScope();

    {
        QWebEngineView& window = *reinterpret_cast<QWebEngineView*> (DartArgs::GetUInt(arguments, 1));
        const char *url = DartArgs::GetString(arguments, 2);
        setUrlWindow(window, url);
    }

    Dart_SetReturnValue(arguments, Dart_Null());
    Dart_ExitScope();
}
}

struct FunctionLookup {
  const char* name;
  Dart_NativeFunction function;
};

FunctionLookup functionList[] = {
  {"App::Init", AppInit},
  {"App::ProcessEvents", AppProcessEvents},
  {"Window::Create", Window::Create},
  {"Window::SetVisibility", Window::SetVisibility},
  {"Window::Resize", Window::Resize},
  {"Window::SetUrl", Window::SetUrl}
};

Dart_NativeFunction ResolveName(Dart_Handle name, int, bool*) {
  if (!Dart_IsString(name)) return NULL;
  Dart_NativeFunction result = NULL;
  Dart_EnterScope();
  const char* cname;
  HandleError(Dart_StringToCString(name, &cname));

  for (FunctionLookup& function: functionList) {
    if (strcmp(function.name, cname) == 0) {
      result = function.function;
      break;
    }
  }
  Dart_ExitScope();
  return result;
}

DART_EXPORT Dart_Handle helium_Init(Dart_Handle parent_library) {
  if (Dart_IsError(parent_library)) { return parent_library; }

  Dart_Handle result_code = Dart_SetNativeResolver(parent_library, ResolveName, NULL);
  if (Dart_IsError(result_code)) return result_code;

  return Dart_Null();
}
#endif
