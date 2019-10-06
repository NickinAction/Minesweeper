#include "field.h"
#include <QTime>
#include <QTimer>
#include <QtWidgets>
#include "mainwindow.h"
#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <QtDebug>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include <QPixmap>

Field::Field(QWidget *parent, char difficulty) : QWidget(parent){
    this->difficulty = difficulty;
}

void Field::paintEvent(QPaintEvent *Event) {

    QPainter painter(this);
    QPixmap flag = QPixmap(":/new/images/flag.png");
    //painter.drawPixmap(10,10,50,50, flag);

    painter.fillRect(0, 0, fieldPixelSize, fieldPixelSize, block_color);

    //difficulty = 'h';//temporary

    if (difficulty == 'e'){
        fieldSize = 10;
    }
    else if (difficulty == 'm') {
        fieldSize = 25;
    }
    else if (difficulty == 'h') {
        fieldSize = 38;
    }

    blockSize = fieldPixelSize/fieldSize;

    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            painter.drawRect(blockSize*i, blockSize*j, blockSize, blockSize);
        }
    }

    painter.drawPixmap(0, 0, blockSize, blockSize, flag);
    //QPoint pos = QCursor::pos();


}

void Field::mousePressEvent(QMouseEvent *e){
    qDebug() << e->button() << " x: " << e->x() << " y: " << e->y() << endl;
}

