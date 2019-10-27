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


    flag = QPixmap(":/images/Minesweeper_flag.svg.png");
    unopened_block = QPixmap(":/images/Minesweeper_unopened_square.svg.png");
    mine = QPixmap(":/images/Gnome-gnomine.png");
    opened_block = QPixmap(":/images/Minesweeper_pressed-square.svg.png");


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
            fieldArray[i][j] = UNOPENED;
        }
    }
}

void Field::paintEvent(__attribute__((unused))QPaintEvent *Event) {

    QPainter painter(this);
    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            switch(fieldArray[i][j]) {
                case UNOPENED:
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, unopened_block);
                break;
                case FLAG:
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, flag);
                break;
            default: //opened
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, opened_block);

            }
        }
    }


}

void Field::mousePressEvent(QMouseEvent *e){

    int MEblockX; // mouse event block
    int MEblockY;

    if (e->button() == Qt::LeftButton) {
        if (withinField(e->x(), e->y())) {
            MEblockX = e->x()/blockSize;
            MEblockY = e->y()/blockSize;

            if(fieldArray[MEblockX][MEblockY] == UNOPENED) {
                fieldArray[MEblockX][MEblockY] = OPENED;
            }

        }

    } else if (e->button() == Qt::RightButton){
        if (withinField(e->x(), e->y())) {
            MEblockX = e->x()/blockSize;
            MEblockY = e->y()/blockSize;

            if(fieldArray[MEblockX][MEblockY] == FLAG) {
                fieldArray[MEblockX][MEblockY] = UNOPENED;
            }
            else if (fieldArray[MEblockX][MEblockY] == UNOPENED) {
                fieldArray[MEblockX][MEblockY] = FLAG;
            }

        }
    }

    qDebug() << e->button() << " x: " << e->x() << " y: " << e->y() << endl;
    this->update();
}


bool Field::withinField(int x, int y) {
    return(x >= 0 && x <= 950 && x >= 0 && y <= 950);
}
