QT       += core gui
QT       += sql         # needed for database
QT       += charts      # needed for graphics
QT       += multimedia  # needed for music

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Ruta relativa para encontrar el archivo .lib
LIBS += -L$$PWD/lib/ -lConnect4Lib

# Ruta relativa para encontrar los archivos de cabecera
INCLUDEPATH += $$PWD/lib/

SOURCES += \
    src/aboutwindow.cpp \
    src/boardplayer.cpp \
    src/gameboard.cpp \
    src/historywindow.cpp \
    src/individualrankingwindow.cpp \
    src/logwindow.cpp \
    src/main.cpp \
    src/mainmenu.cpp \
    src/mainwindow.cpp \
    src/newgamepart2window.cpp \
    src/newgamewindow.cpp \
    src/optionswindow.cpp \
    src/profilesidemenu.cpp \
    src/profilewindow.cpp \
    src/rankingwindow.cpp \
    src/singwindow.cpp \
    src/startwindow.cpp \
    src/validation.cpp \
    src/welcomewindow.cpp

HEADERS += \
    src/aboutwindow.h \
    src/boardplayer.h \
    src/gameboard.h \
    src/historywindow.h \
    src/individualrankingwindow.h \
    src/logwindow.h \
    src/mainmenu.h \
    src/mainwindow.h \
    src/newgamepart2window.h \
    src/newgamewindow.h \
    src/optionswindow.h \
    src/profilesidemenu.h \
    src/profilewindow.h \
    src/rankingwindow.h \
    src/singwindow.h \
    src/startwindow.h \
    src/validation.h \
    src/welcomewindow.h

FORMS += \
    src/aboutwindow.ui \
    src/gameboard.ui \
    src/historywindow.ui \
    src/individualrankingwindow.ui \
    src/logwindow.ui \
    src/mainmenu.ui \
    src/mainwindow.ui \
    src/newgamepart2window.ui \
    src/newgamewindow.ui \
    src/optionswindow.ui \
    src/profilesidemenu.ui \
    src/profilewindow.ui \
    src/rankingwindow.ui \
    src/singwindow.ui \
    src/startwindow.ui \
    src/welcomewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
