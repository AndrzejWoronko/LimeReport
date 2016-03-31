include(../common.pri)
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4){
    QT += widgets printsupport
    DEFINES += HAVE_QT5
}

macx{
    CONFIG  += app_bundle
}

TARGET = LRDemo
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

INCLUDEPATH += $$PWD/../include
DEPENDPATH  += $$PWD/../include

RESOURCES += \
    r1.qrc

EXTRA_DIR += $$PWD/demo_reports/*
DEST_DIR       = $${BUILD_DIR}/$${BUILD_TYPE}/demo
REPORTS_DIR    = $${DEST_DIR}/demo_reports

unix:{
    LIBS += -L$${BUILD_DIR}/$${BUILD_TYPE}/lib -llimereport
    contains(CONFIG,zint){
        LIBS += -L$${BUILD_DIR}/$${BUILD_TYPE}/lib -lQtZint
    }
    DESTDIR = $$DEST_DIR
    QMAKE_POST_LINK += mkdir -p $$quote($$REPORTS_DIR) | $$QMAKE_COPY_DIR $$quote($$EXTRA_DIR) $$quote($$REPORTS_DIR) $$escape_expand(\n\t)
!macx{
    #Link share lib to ../lib rpath
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/lib
    QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN/../lib
    QMAKE_LFLAGS_RPATH += #. .. ./libs
}
    target.path = $${DEST_DIR}
    INSTALLS = target
}

win32 {
    EXTRA_DIR ~= s,/,\\,g
    DEST_DIR ~= s,/,\\,g
    REPORTS_DIR ~= s,/,\\,g

    DESTDIR = $$DEST_DIR
    RC_FILE += mainicon.rc

    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$quote($$EXTRA_DIR) $$quote($$REPORTS_DIR) $$escape_expand(\\n\\t)
    LIBS += -L$${BUILD_DIR}/$${BUILD_TYPE}/lib -llimereport
}
