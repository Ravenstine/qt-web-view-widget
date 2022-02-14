**NOTE: This library inherits from private Qt modules.  Use with caution.**

Qt WebView Widget
=================

A simple Qt widget for embedding a web view without the need for Quick or WebEngine.

## Motivation

Why embed Blink or WebKit when the host already has a browser engine?

Qt comes with a web view QML widget out of the box, but you are mostly limited to interacting with it in the QML context.  The developers of Qt don't seem to think anyone would want to do things like call `runJavaScript` from C++.

So I came up with this widget that just does one thing, which is embed a web view through C++ and supports things like executing JavaScript with callbacks.  The widget currently still uses the underlying components of QtWebView by reaching into its protected/private members, making this library very small.

## Prerequisites

- Qt >= 6.x
- qmake

## Installation

```sh
git clone https://github.com/ravenstine/qt-web-view-widget.git

cd qt-web-view-widget

qmake
make
make install
```

This will create a `dist/` folder with a `lib` and `include` path.  You can leave this in place or copy the files wherever they need to go on your system.  I don't really want to bother handling individual install paths for different operating systems.

If you are using Qt Creator, you can right-click on the name of your project in the Projects pane and select **Add Library**.  Choose **External Library**, select the `.o`/`.so`/`.a`/`.dylib`/`.dll` file, and make sure to specify the `include/` directory path for the headers.

You can do the same thing manually in your `.pro` file, depending on where you compiled the library, with something like this:

```qmake
LIBS += -L$$PWD/../qt-web-view-widget/dist/lib/ -lwebviewwidget
INCLUDEPATH += $$PWD/../qt-web-view-widget/dist/include
DEPENDPATH += $$PWD/../qt-web-view-widget/dist/include
```

Also make sure you have the following:

```qmake
QT += widgets webview-private
```

### Usage

You can now include the library in your C++ code:

```cpp
#include <qtwebviewwidget/widget.h>
```

The class `WebViewWidget` will then be available for you to use.


## WebViewWidget

**Inherits from [QWidget](https://doc.qt.io/qt-6/qwidget.html)**

Use this widget to instantiate a web view using the available browser engine on the host.  The view will fill the entire space of the widget and resize with it.

Example:

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <qtwebviewwidget/widget.h>

QT_USE_NAMESPACE

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow window;
  WebViewWidget webViewWidget(QUrl(QString("https://www.qt.io/")));

  window.setCentralWidget(&webViewWidget);
  window.show();

  return app.exec();
}
```

Most of the methods you'll be using for interacting with the web context are on the `webView` member.

## WebViewWidget Methods

### `WebViewWidget::WebViewWidget()`
--------------
### `WebViewWidget::WebViewWidget(const QUrl &url)`
--------------
### `WebViewWidget::WebViewWidget(const QString &html, const QUrl &baseUrl)`
--------------
### `void init()`

This inserts the WebView into the widget's internal window and initializes it.  It is called automatically when the widget is inserted.

The widget waits until it insertion to initialize the view to prevent it from doing things like navigate and fire off requests before the developer has fully configured it.  However, this usually causes a slight delay between the Qt app rendering on screen and the web view actually being visible.

If you want the web view to appear at the same time as the app, call `init()` to kick off invoking the web view before the app is painted.

## WebView

This class is primarily a derivative of the private `QWebView` C++ class but elevates it to being public.  It is nearly the same as the QML WebView widget, the API of which you can [read about here](https://doc.qt.io/qt-6/qml-qtwebview-webview-members.html), but with more methods exposed including a few slight modifications to make it more useful outside of a QML context.

The reason this is a separate class from `WebViewWidget` is so that you can integrate a web view in other contexts besides a widget.

In summary, you will get a lot of the basic functions of working with a web browser, but there are no native ways to listen to events or intercept requests, although those things are theoretically possible by emulating it with injected JavaScript.  The reason why things like [QtWebEngine](https://doc.qt.io/qt-6/qtwebengine-index.html) exists is to overcome these issues and improve consistency across platforms.

## WebView Methods

### `void init()`

I don't know precisely what this is supposed to do, but it's there because it might be used to initialize something for the host web view browser engine.

--------------
### `void runJavaScript(const QString &script)`
--------------
### `void runJavaScript(const QString &script, std::function<void(const QVariant &variant)> callback)`

The `runJavaScript` method is nearly the same as the one exposed within the QML JavaScript engine, except you can use it in C++ and pass a function pointer or lambda as a callback that will be performed when the provided script is finished performing.

Example:

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <qtwebviewwidget/widget.h>

QT_USE_NAMESPACE

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow window;
  WebViewWidget webViewWidget(QUrl(QString("https://www.qt.io/")));

  window.setCentralWidget(&webViewWidget);

  webViewWidget.webView->runJavaScript("'foobar';", [&] (const QVariant &result) {
    qDebug() <<  result.toString();
  });

  window.show();

  return app.exec();
}
```
--------------
### `QString httpUserAgent()`
--------------
### `void setHttpUserAgent(const QString &httpUserAgent)`
--------------
### `QUrl url()`
--------------
### `void setUrl(const QUrl &url)`
--------------
### `bool canGoBack()`
--------------
### `bool canGoForward()`
--------------
### `QString title()`
--------------
### `int loadProgress()`
--------------
### `bool isLoading()`
--------------
### `void setParentView(QObject *view)`
--------------
### `QObject *parentView()`
--------------
### `void setGeometry(const QRect &geometry)`
--------------
### `void setVisibility(QWindow::Visibility visibility)`
--------------
### `void setVisible(bool visible)`
--------------
### `void setFocus(bool focus)`
--------------

#### WebView Signals

Although not stated in the official documentation, the `WebView` does support other signals that can be useful for responding to page state.

### `titleChanged(void)`
--------------
### `urlChanged(void)`
--------------
### `loadingChanged(const WebViewLoadRequest &loadRequest)`

See [https://doc.qt.io/qt-6/qml-qtwebview-webview.html#loadingChanged-signal](loadingChanged signal).

Example:

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <qtwebviewwidget/widget.h>

QT_USE_NAMESPACE

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QMainWindow window;
  WebViewWidget webViewWidget(QUrl(QString("https://www.qt.io/")));

  window.setCentralWidget(&webViewWidget);

  QObject::connect(webViewWidget.webView, &WebView::loadingChanged, [] (const WebViewLoadRequest &loadRequest) {
      qDebug() << &loadRequest;
  });

  window.show();

  return app.exec();
}
```

`WebViewLoadRequest` is just a public alias for `QWebViewLoadRequestPrivate`.

...
--------------
### `loadProgressChanged(void)`
--------------
### `javaScriptResult(int id, const QVariant &result)`

This is a low-level signal method, you probably don't want to use it, but it's there.

### `requestFocus(bool focus)`
--------------
### `httpUserAgentChanged(void)`
--------------
### `cookieAdded(const QString &domain, const QString &name)`
--------------
### `cookieRemoved(const QString &domain, const QString &name)`

## License

See [LICENSE.txt](LICENSE.txt)