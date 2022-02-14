#ifndef WEB_VIEW_WIDGET_H
#define WEB_VIEW_WIDGET_H

#include "webview.h"
#include <QString>
#include <QUrl>
#include <QWidget>
#include <QWindow>

class WebViewWidget : public QWidget {
    Q_OBJECT
public:
    WebViewWidget();
    WebViewWidget(const QUrl &url);
    WebViewWidget(const QString &html, const QUrl &baseUrl = QUrl());
    ~WebViewWidget();

    WebView * webView = nullptr;
private:
    QWindow * internalWindow = nullptr;
    QWidget * windowContainer = nullptr;
    void onDimensionChange();
    void setupWidget();
    bool didInsert = false;
};

#endif // WEB_VIEW_WIDGET_H
