#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "gamewindow.h"
#include <QFileDialog>
#include <QMessageBox>

mainMenu::mainMenu(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainMenu) {
    ui->setupUi(this);

    connect(ui->exitButton, &QPushButton::clicked, this, &QMainWindow::close);
    connect(ui->exitButton, &QPushButton::clicked, this, &QMainWindow::deleteLater);
}

mainMenu::~mainMenu() {
    delete ui;
}

void mainMenu::on_playButton_clicked() {
    this->hide();

    m_gameWindow = new GameWindow();
    connect(m_gameWindow, &QObject::destroyed, this, [this]() {
        show();
    });

    bool canBeStarted = m_gameWindow->startGame(m_selectedMap);
    if (!canBeStarted) {
        m_gameWindow->close();
        return;
    }

    m_gameWindow->show();
}

void mainMenu::on_chooseMapButton_clicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Select Maze File",
        QDir::currentPath(),
        "Text Files (*.txt);;All Files (*)"
        );

    if (!fileName.isEmpty()) {
        // Validate it's a TXT file
        if (fileName.endsWith(".txt", Qt::CaseInsensitive)) {
            m_selectedMap = fileName;
            ui->mapLabel->setText(QFileInfo(fileName).fileName());
        } else {
            QMessageBox::warning(this, "Invalid File",
                                 "Please select a .txt file");
        }
    }
}

