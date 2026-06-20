#include "ui/WebViewManager.h"
#ifdef _WIN32
#if defined(__has_include)
#if __has_include(<webview.h>)
#include <webview.h>
#elif __has_include(<webview/webview.h>)
#include <webview/webview.h>
#elif __has_include(<webview2.h>)
#include <webview2.h>
#include <wrl/client.h>
using namespace Microsoft::WRL;
#else
#error "Neither <webview2.h> nor <webview/webview.h> found"
#endif
#else
#include <webview/webview.h>
#endif
#else
#include <webview/webview.h>
#endif

namespace ETL {
namespace ui {}  // namespace ui
}  // namespace ETL