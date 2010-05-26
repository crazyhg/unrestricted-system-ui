include(../common_top.pri)
TARGET = ut_lockscreenbusinesslogic

SRC_PREFIX = ../../src
STUB_PREFIX = ../stubs

INCLUDEPATH += $$SRC_PREFIX \
               $$STUB_PREFIX

SOURCES += ut_lockscreenbusinesslogic.cpp \
    $$SRC_PREFIX/debug.cpp \
    $$SRC_PREFIX/lockscreenbusinesslogic.cpp \
    $$SRC_PREFIX/unlockwidgets.cpp \
    $$SRC_PREFIX/lockscreenui.cpp  \
    $$STUB_PREFIX/stubbase.cpp \
    $$SRC_PREFIX/unlocknotifications.cpp

HEADERS += ut_lockscreenbusinesslogic.h \
    $$SRC_PREFIX/debug.h \
    $$SRC_PREFIX/lockscreenbusinesslogic.h \
    $$SRC_PREFIX/unlockwidgets.h \
    $$SRC_PREFIX/lockscreenui.h \
    $$SRC_PREFIX/sysuid.h \
    $$SRC_PREFIX/unlocknotifications.h

include(../common_bot.pri)
