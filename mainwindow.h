#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "field.h"
#include "startgamedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void setDifficulty(char diff);

private:
    Ui::MainWindow *ui;
    Field *field = new Field();
    StartGameDialog* dialog;

};

#endif // MAINWINDOW_H
