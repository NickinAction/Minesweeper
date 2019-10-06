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

private:

    QColor block_color = QColor(170, 170, 170, 255);
    QColor field_color = QColor(187,187,187, 255);
    Ui::MainWindow *ui;
    Field *field = new Field();
    StartGameDialog* dialog;

};

#endif // MAINWINDOW_H
