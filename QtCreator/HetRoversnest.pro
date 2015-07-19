win32 {
  # Windows only
  message("Console application, built for Windows")
  QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++
}

macx {
  # Mac only
  message("Console application, built for Mac")
  QMAKE_CXXFLAGS = -mmacosx-version-min=10.7 -std=gnu0x -stdlib=libc+
  CONFIG +=c++11
}

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  QMAKE_CXXFLAGS += -Werror
  QMAKE_CXXFLAGS += -std=c++1y -Wall -Wextra -Weffc++
}

cross_compile {
  # Crosscompile only
  message("Console application, cross-compiling from Linux to Windows")
  QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++
}


# Go ahead and use Qt.Core: it is about as platform-independent as
# the STL and Boost
QT += core

# Go ahead and use Qt.Gui: it is about as platform-independent as
# the STL and Boost. It is needed for QImage
QT += gui

# Don't define widgets: it would defy the purpose of this console
# application to work non-GUI
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

#
#
# Type of compile
#
#

CONFIG(release, debug|release) {
  DEFINES += NDEBUG NTRACE_BILDERBIKKEL
}

SOURCES += main.cpp \
    helper.cpp \
    dochapter.cpp \
    character.cpp \
    item.cpp \
    test.cpp \
    autoplay.cpp \
    language.cpp \
    monster.cpp \
    chapter.cpp \
    option.cpp \
    chaptertype.cpp \
    fightingchapter.cpp \
    optionschapter.cpp \
    condition.cpp \
    consequence.cpp \
    luckchapter.cpp \
    skillchapter.cpp \
    shopchapter.cpp \
    pawnshopchapter.cpp

HEADERS += \
    helper.h \
    dochapter.h \
    character.h \
    item.h \
    autoplay.h \
    language.h \
    monster.h \
    chapter.h \
    option.h \
    chaptertype.h \
    fightingchapter.h \
    optionschapter.h \
    condition.h \
    consequence.h \
    luckchapter.h \
    skillchapter.h \
    shopchapter.h \
    pawnshopchapter.h
