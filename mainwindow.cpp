#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QKeyEvent>
#include <QtDebug>
#include <QPainter>
#include "numberdisplay.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dialog = new StartGameDialog(this);
    connect(dialog, SIGNAL(setDifficulty(char)), this, SLOT(setDifficulty(char)));


    //connect(this->ui->smileButton, SIGNAL(clicked()), this->dialog, SLOT(show()));
    QPixmap smile = QPixmap(":/images/smile.png");
    QIcon ButtonIcon(smile);
    this->ui->smileButton->setIcon(ButtonIcon);
    this->ui->smileButton->setIconSize(this->ui->smileButton->size());


    flagDisplay = new NumberDisplay(this);
    timerDisplay = new NumberDisplay(this);

    //connect(obj1, SIGNAL(someSignal(arg types)), obj2, SLOT(someSlot(arg types)));
    connect(this, SIGNAL(updateFlagCount(int)), flagDisplay, SLOT(setFlagNum(int)));

    ui->numberDisplayLayout->addWidget(flagDisplay);
    //ui->numberDisplayLayout->addWidget(timerDisplay);
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
    connect(field, SIGNAL(sendFlagCount(int)), this, SLOT(setFlagCount(int)));
    ui->verticalLayout->addWidget(field);
    setFlagCount(field->getFlagCount());
}

void MainWindow::on_smileButton_clicked(){
    this->dialog->show();
    //this->ui->smileButton->setText("Reset");
}

void MainWindow::setFlagCount(int flagCount) {
    //this->ui->label->setText(QString::fromStdString(std::to_string(flagCount)));
    this->ui->label->setText(QString::fromStdString(std::to_string(flagCount)));
    emit updateFlagCount(flagCount);
}
