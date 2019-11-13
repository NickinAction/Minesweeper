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
#include <random>

Field::Field(QWidget *parent, char difficulty) : QWidget(parent){
    this->difficulty = difficulty;


    flag = QPixmap(":/images/Minesweeper_flag.svg.png");
    unopened_block = QPixmap(":/images/Minesweeper_unopened_square.svg.png");
    mine = QPixmap(":/images/Gnome-gnomine.png");
    opened_block = QPixmap(":/images/Minesweeper_pressed-square.svg.png");
    number1 = QPixmap(":/images/number1.png");
    number2 = QPixmap(":/images/number2.png");
    number3 = QPixmap(":/images/number3.png");
    number4 = QPixmap(":/images/number4.png");
    number5 = QPixmap(":/images/number5.png");
    number6 = QPixmap(":/images/number6.png");
    number7 = QPixmap(":/images/number7.png");
    number8 = QPixmap(":/images/number8.png");


    // This code just asksfor switch/case instead.
    if (difficulty == 'e'){
        fieldSize = 10;
        mineCount = 12;
    }
    else if (difficulty == 'm') {
        fieldSize = 25;
        mineCount = 94;
    }
    else if (difficulty == 'h') {
        fieldSize = 38;
        mineCount = 216;
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

    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            switch(hiddenFieldArray[i][j]) {
                case '1':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number1);
                    break;
                case '2':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number2);
                    break;
                case '3':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number3);
                    break;
                case '4':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number4);
                    break;
                case '5':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number5);
                    break;
                case '6':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number6);
                    break;
                case '7':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number7);
                    break;
                case '8':
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, number8);
                    break;

            }
            if(hiddenFieldArray[i][j] == MINE) {
                painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, mine);
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

            if(firstClick) {
                generateHiddenField(MEblockX, MEblockY);
                firstClick = false;
            }

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

void Field::openFieldSection(int x, int y) { //wave agl

}


bool Field::withinField(int x, int y) {
    return(x >= 0 && x <= 950 && x >= 0 && y <= 950);
}

void Field::generateHiddenField(int x, int y) {
    srand (time(nullptr));
    for(int i = 0; i < mineCount; i++) {
        int rand1, rand2;

        do {
            rand1 = rand() % fieldSize;
            rand2 = rand() % fieldSize;
        }while(hiddenFieldArray[rand1][rand2] == MINE || (abs(x - rand1) < 2 && abs(rand2 - y) < 2));

        hiddenFieldArray[rand1][rand2] = MINE;
    }

    int numberOfMines;
    for (int i = 0; i < fieldSize; i++) {
        for(int j = 0; j < fieldSize; i++) {
            numberOfMines = 0;
            if (j != fieldSize) { //
                if (hiddenFieldArray[i+1][j] == MINE) { // DOWN
                    numberOfMines++;
                }
            }
            if (j != 0) {
                if (hiddenFieldArray[i-1][j] == MINE) { // UP --
                    numberOfMines++;
                }
            }
            if (i != fieldSize) {
                if (hiddenFieldArray[i][j+1] == MINE) { // RIGHT
                    numberOfMines++;
                }
            }
            if (i != 0) {
                if (hiddenFieldArray[i][j-1] == MINE) { // LEFT--
                    numberOfMines++;
                }
            }
            if (i != fieldSize && j != 0) {
                if (hiddenFieldArray[i-1][j+1] == MINE) { // UP-RIGHT--
                    numberOfMines++;
                }
            }
            if (i != fieldSize && j != fieldSize) {
                if (hiddenFieldArray[i+1][j+1] == MINE) { // DOWN-RIGHT
                    numberOfMines++;
                }
            }
            if (i != 0 && j != 0) {
                if (hiddenFieldArray[i-1][j-1] == MINE) { // UP-LEFT--
                    numberOfMines++;
                }
            }
            if (i != 0 && j != fieldSize) {
                if (hiddenFieldArray[i+1][j-1] == MINE) { // DOWN-LEFT--
                    numberOfMines++;
                }
            }

            hiddenFieldArray[i][j] = numberOfMines;
        }

    }
}
