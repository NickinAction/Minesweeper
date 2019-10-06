#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "field.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtDebug>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    field = new Field();
    dialog = new StartGameDialog(this);
    connect(dialog, SIGNAL(setDifficulty(char)), this, SLOT(setDifficulty(char)));
    ui->verticalLayout->addWidget(field);

    connect(this->ui->startButton, SIGNAL(clicked()), this->dialog, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDifficulty(char diff) {
    difficulty = diff;
    qDebug() << "Difficulty: " << difficulty << endl;

}

















