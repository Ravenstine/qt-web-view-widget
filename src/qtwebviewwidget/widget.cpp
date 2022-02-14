#include "widget.h"
#include <QString>
#include <QUrl>
#include <QWidget>
#include <QWindow>
#include <QVBoxLayout>

WebViewWidget::WebViewWidget(const QUrl &url) {
    setupWidget();
    webView->setUrl(url);
}

WebViewWidget::WebViewWidget(const QString &html, const QUrl &baseUrl) {
    setupWidget();
    webView->loadHtml(html, baseUrl);
}

WebViewWidget::WebViewWidget() {
    setupWidget();
}

void WebViewWidget::setupWidget() {
    webView = new WebView;
    internalWindow = new QWindow;
    QWidget * webViewWidget = QWidget::createWindowContainer(internalWindow);

    this->setLayout(new QVBoxLayout);
    this->layout()->addWidget(webViewWidget);
    this->layout()->setContentsMargins(0, 0, 0, 0);

    // Handle parent size changes
    QObject::connect(internalWindow, &QWindow::widthChanged, this, &WebViewWidget::onDimensionChange);
    QObject::connect(internalWindow, &QWindow::heightChanged, this, &WebViewWidget::onDimensionChange);
    QObject::connect(internalWindow, &QWindow::xChanged, this, &WebViewWidget::onDimensionChange);
    QObject::connect(internalWindow, &QWindow::yChanged, this, &WebViewWidget::onDimensionChange);
    QObject::connect(internalWindow, &QWindow::visibleChanged, this, [this](bool visible) {
        webView->setVisible(visible);
    });

    this->onDimensionChange();

    webView->setParentView(internalWindow);
    webView->setVisibility(QWindow::Visibility::Windowed);
    webView->init();
}

void WebViewWidget::onDimensionChange() {
    webView->setGeometry(
        QRect(QPoint(0, 0), QSize(internalWindow->width(), internalWindow->height()))
    );
}

WebViewWidget::~WebViewWidget() {
    internalWindow->close();
}
