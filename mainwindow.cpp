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

    //QRect frame = ui->fieldFrame->geometry();


    QColor block_color = QColor(155, 170, 170, 255);
    QColor field_color = QColor(187,187,187, 255);

    field = new Field();
    ui->verticalLayout->addWidget(field);

}

MainWindow::~MainWindow()
{
    delete ui;
}
