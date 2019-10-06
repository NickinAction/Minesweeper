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
    dialog = new StartGameDialog(this);
    connect(dialog, SIGNAL(setDifficulty(char)), this, SLOT(setDifficulty(char)));
    connect(this->ui->startButton, SIGNAL(clicked()), this->dialog, SLOT(show()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDifficulty(char diff) {
    if (field) {
        field->close();
    }
    field = new Field(this, diff);
    ui->verticalLayout->addWidget(field);
}

















