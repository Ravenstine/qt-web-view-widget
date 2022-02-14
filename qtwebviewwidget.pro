TARGET = qtwebviewwidget
VERSION=0.0.1
TEMPLATE = lib
QT += widgets webview-private
CONFIG += shared
SOURCES += \
    src/qtwebviewwidget/widget.cpp \
    src/qtwebviewwidget/webview.cpp
HEADERS += \
    src/qtwebviewwidget/widget.h \
    src/qtwebviewwidget/webview.h
DESTDIR = ./build
INSTALLS += target header_files

target.path = ./dist/lib
header_files.files = $$HEADERS
header_files.path = ./dist/include/qtwebviewwidget
