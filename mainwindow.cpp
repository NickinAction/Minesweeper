#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QTime>
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


    QPixmap smile = QPixmap(":/images/smile.png");
    QIcon ButtonIcon(smile);
    this->ui->smileButton->setIcon(ButtonIcon);
    this->ui->smileButton->setIconSize(this->ui->smileButton->size());


    flagDisplay = new NumberDisplay(this);
    timerDisplay = new NumberDisplay(this);

    connect(this, SIGNAL(updateFlagCount(int)), flagDisplay, SLOT(setNum(int)));
    connect(this, SIGNAL(updateTimer(int)), timerDisplay, SLOT(setNum(int)));

    ui->numberDisplayLayout->addWidget(flagDisplay);
    ui->numberDisplayLayout->addWidget(timerDisplay);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::setDifficulty(char diff) {
    if (field) {
        field->close();
        timerCount = 0;
        tickTimer = false;
        emit updateTimer(0);
    }
    field = new Field(this, diff);
    connect(field, SIGNAL(sendFlagCount(int)), this, SLOT(setFlagCount(int)));
    connect(field, SIGNAL(startGameTimer(bool)), this, SLOT(timerTick(bool)));
    connect(field, SIGNAL(resizeInLayout(int,int)), this, SLOT(updateFieldHeight(int,int)));
    connect(field, SIGNAL(updateGameStatus(char)), this, SLOT(gameOver(char)));
    ui->fieldLayout->addWidget(field);
    setFlagCount(field->getFlagCount());
}

void MainWindow::updateFieldHeight(int width, int height) {
    if (field) {
        this->ui->fieldLayout->geometry().setX(width);
        this->ui->fieldLayout->geometry().setY(height);
    }
}

void MainWindow::timerTick(bool fromField) {
    if(fromField || tickTimer) {
        emit updateTimer(timerCount++);
        timer->start(1000);
        tickTimer = true;
    }
}

void MainWindow::on_smileButton_clicked(){
    this->dialog->show();
}

void MainWindow::gameOver(char gameStatus) {
    if(gameStatus == 'w') {
        //set smile to cool glasses version

    }
    else if(gameStatus == 'l') {
        //set smile to dead version

    }

    timer->stop();
}

void MainWindow::setFlagCount(int flagCount) {
    this->ui->label->setText(QString::fromStdString(std::to_string(flagCount)));
    emit updateFlagCount(flagCount);
}
