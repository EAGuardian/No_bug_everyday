#-------------------------------------------------
# Project created by QtCreator 2019-08-31T09:35:17
#-------------------------------------------------
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = bug_everyday_ide_complite
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
SOURCES += \
        main.cpp \
        mainwindow.cpp \
        find_dialog.cpp
HEADERS += \
        mainwindow.h \
    filedeal.h \
    myfile.h \
    find_dialog.h \
    basicoperation.h
FORMS += \
        mainwindow.ui \
        find_dialog.ui
DISTFILES +=
RESOURCES += \
    res.qrc
INCLUDEPATH += C:\Users\Administrator\Documents\QT\QScintilla_gpl-2.11.2\Qt4Qt5
win32:CONFIG(release, debug|release): LIBS += -LC:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/release/ -lqscintilla2_qt5
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/debug/ -lqscintilla2_qt5
else:unix: LIBS += -LC:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/ -lqscintilla2_qt5

INCLUDEPATH += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/release
DEPENDPATH += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/release/libqscintilla2_qt5.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/debug/libqscintilla2_qt5.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/release/qscintilla2_qt5.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/debug/qscintilla2_qt5.lib
else:unix: PRE_TARGETDEPS += C:/Users/Administrator/Documents/QT/QScintilla_gpl-2.11.2/build-qscintilla-Desktop_Qt_5_10_1_MinGW_32bit-Release/libqscintilla2_qt5.a
