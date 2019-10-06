#include "startgamedialog.h"
#include "ui_startgamedialog.h"

StartGameDialog::StartGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartGameDialog)
{
    ui->setupUi(this);
    connect(this->ui->okButton, SIGNAL(clicked()), this, SLOT(close()));
}

StartGameDialog::~StartGameDialog()
{
    delete ui;
}
