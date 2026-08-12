#pragma once
#include <QMainWindow>

class GameWindow;

QT_BEGIN_NAMESPACE
namespace Ui {
class mainMenu;
}
QT_END_NAMESPACE

class mainMenu : public QMainWindow {
    Q_OBJECT

public:
    mainMenu(QWidget *parent = nullptr);
    ~mainMenu();

private slots:
    void on_playButton_clicked();
    void on_chooseMapButton_clicked();

private:
    Ui::mainMenu *ui;
    GameWindow* m_gameWindow;
    QString m_selectedMap = "maze2.txt";
};
