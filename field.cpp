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

Field::Field(QWidget *parent, char difficulty) : QWidget(parent){
    this->difficulty = difficulty;
}

void Field::paintEvent(QPaintEvent *Event) {

    QPainter painter(this);
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

    //QPoint pos = QCursor::pos();


}

