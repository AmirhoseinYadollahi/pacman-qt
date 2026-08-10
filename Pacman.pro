QT       += core gui widgets svg

CONFIG += c++17

# Uncomment to enforce Qt 6 compatibility by disabling deprecated APIs
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    gamewindow.cpp \
    ghost.cpp \
    main.cpp \
    mainmenu.cpp \
    mazeloader.cpp \
    pacman.cpp \
    wall.cpp

HEADERS += \
    gamewindow.h \
    ghost.h \
    mainmenu.h \
    mazeloader.h \
    pacman.h \
    wall.h

FORMS += \
    mainmenu.ui

DISTFILES += maze1.txt

# Include the resource file if you have images or sounds
RESOURCES += \
    Images.qrc

# Deployment rules
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
