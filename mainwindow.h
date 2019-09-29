#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include "field.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:


public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //QRect frame = ui->field->geometry();

    QColor block_color = QColor(170, 170, 170, 255);
    QColor field_color = QColor(187,187,187, 255);

    Field *field = new Field();

private:

    //void paintEvent(QPaintEvent *Event);
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
