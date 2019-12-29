#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "field.h"
#include "startgamedialog.h"
#include "numberdisplay.h"

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

private slots:
    void on_smileButton_clicked();
    void setFlagCount(int flagCount);

signals:
    void updateFlagCount(int number);

private:
    Ui::MainWindow *ui;
    Field *field = nullptr;
    NumberDisplay* timerDisplay;
    NumberDisplay* flagDisplay;
    StartGameDialog* dialog;
};

#endif // MAINWINDOW_H
