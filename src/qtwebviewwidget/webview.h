#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QString>
#include <QVariant>
#include <QtWebView/private/qwebview_p.h>
#include <QtWebView/qtwebviewfunctions.h>
#include <QtWebView/private/qwebviewloadrequest_p.h>

typedef std::function<void(const QVariant &variant)> WebViewJSCallback;

using WebViewLoadRequest = QWebViewLoadRequestPrivate;

class WebView : public QWebView {
public:
    WebView();
    void runJavaScript(const QString &script);
    void runJavaScript(const QString &script, WebViewJSCallback callback);
    void init() override;
private:
    void onRunJavaScriptResult(int id, const QVariant &variant);
};

#endif // WEBVIEW_H
