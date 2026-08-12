#include "mainmenu.h"
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    mainMenu menu;
    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    menu.move((screenGeometry.width() - menu.width()) / 2,
              (screenGeometry.height() - menu.height()) / 2);

    menu.show();

    return app.exec();
}
