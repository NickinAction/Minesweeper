#include "field.h"
#include <QTime>
#include <QTimer>
// Will you require timers both in MainWindow and Field?
// If think that field doesn't really need it.
// If not, you can remove extra imports.
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
    // This code just asksfor switch/case instead.
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

    for(int i = 0; i < fieldSize; i++) {
        for(int j = 0; j < fieldSize; j++) {
            fieldArray[i][j] = 'u';
        }
    }
}

void Field::paintEvent(__attribute__((unused))QPaintEvent *Event) {

    QPainter painter(this);
    QPixmap flag = QPixmap(":/new/images/flag.png");

    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            switch(fieldArray[i][j]) {
                case 'u':
                painter.fillRect(blockSize*i, blockSize*j, blockSize, blockSize, block_color);\
                painter.drawRect(blockSize*i, blockSize*j, blockSize, blockSize);// will be changed in the future (Pixmap)


            }

        }
    }

    painter.drawPixmap(0, 0, blockSize, blockSize, flag);
}

void Field::mousePressEvent(QMouseEvent *e){

    if (e->button() == Qt::LeftButton) {

    } else if (e->button() == Qt::RightButton){
        if (e->x() >= 0 && e->x() <= 950 && e->y() >= 0 && e->y() <= 950) {
            mouseEventblockX = e->x()/blockSize;
            mouseEventblockY = e->y()/blockSize;

            drawflag = true; // REDO THIS
        }
    }

    qDebug() << e->button() << " x: " << e->x() << " y: " << e->y() << endl;
}
