#include "startgamedialog.h"
#include "ui_startgamedialog.h"

StartGameDialog::StartGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartGameDialog)
{
    ui->setupUi(this);
    connect(this->ui->easyButton, SIGNAL(clicked()), this, SLOT(easyButton()));
    connect(this->ui->mediumButton, SIGNAL(clicked()), this, SLOT(mediumButton()));
    connect(this->ui->hardButton, SIGNAL(clicked()), this, SLOT(hardButton()));
}

StartGameDialog::~StartGameDialog()
{
    delete ui;
}


void StartGameDialog::easyButton() {
    emit(setDifficulty('e'));
    close();

}

void StartGameDialog::mediumButton() {
    emit(setDifficulty('m'));
    close();
}

void StartGameDialog::hardButton() {
    emit(setDifficulty('h'));
    close();
}

