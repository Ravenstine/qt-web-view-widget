#include "webview.h"
#include <QObject>
#include <QString>
#include <QHash>
#include <QtCore/qmutex.h>
#include <QVariant>

namespace {
    class CallbackStorage {
    public:
        int insertCallback(WebViewJSCallback &callback) {
            QMutexLocker locker(&m_mtx);
            const int nextId = qMax(++m_counter, 0);
            if (nextId == 0)
                m_counter = 1;

            m_callbacks.insert(nextId, callback);
            return nextId;
        }
        WebViewJSCallback takeCallback(int callbackId) {
            QMutexLocker lock(&m_mtx);
            return m_callbacks.take(callbackId);
        }
    private:
        QMutex m_mtx;
        int m_counter;
        QHash<int, WebViewJSCallback> m_callbacks;
    };
}

Q_GLOBAL_STATIC(CallbackStorage, callbacks);

WebView::WebView() {
    // WebView::javaScriptResult is implemented by the OS-specific plugin.
    // Since we're currently not completely forking the code of QWebView,
    // we'll just hook into that signal and forward it to our own method.
    QObject::connect(this, &WebView::javaScriptResult, this, &WebView::onRunJavaScriptResult);
}

void WebView::runJavaScript(const QString &script) {
    runJavaScriptPrivate(script, -1);
}

void WebView::runJavaScript(const QString &script, WebViewJSCallback callback) {
    const int callbackId = callbacks->insertCallback(callback);
    runJavaScriptPrivate(script, callbackId);
}

void WebView::init() {
    QWebView::init();
}

void WebView::onRunJavaScriptResult(int id, const QVariant &variant) {
    if (id == -1)
        return;

    WebViewJSCallback callback = callbacks->takeCallback(id);

    if (!callback || callback == nullptr)
        return;

    callback(variant);
}
