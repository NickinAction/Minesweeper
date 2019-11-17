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

    number_images.push_back(QPixmap(":/images/Minesweeper_pressed-square.svg.png"));

    for (int i = 1; i <= 8; i++) {
        std::string number_images_address = ":/images/number" + std::string(1, char('0' + i)) + ".png";
        number_images.push_back(QPixmap(QString::fromStdString(number_images_address)));
    }

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
    if (firstClick) { // field hasn't been generated yet
        for (int i = 0; i < fieldSize; i++) {
            for (int j = 0; j < fieldSize; j++) {
                painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, unopened_block);
            }
        }
        return;
    }
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
                    painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1,
                                       number_images[hiddenFieldArray[i][j]]);

            }
        }
    }

    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            if(hiddenFieldArray[i][j] == MINE) {
                painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1, mine);
                continue;
            }
            else {
                qDebug() << hiddenFieldArray[i][j] << endl;
                painter.drawPixmap(blockSize*i+1, blockSize*j+1, blockSize-1, blockSize-1,
                               number_images[hiddenFieldArray[i][j]]);
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

    for (int y = 0; y < fieldSize; y ++) {
        for (int x = 0; x < fieldSize; x++) {
            hiddenFieldArray[y][x] = 0;
        }
    }
    for(int i = 0; i < mineCount; i++) {
        int rand1, rand2;

        do {
            rand1 = rand() % fieldSize;
            rand2 = rand() % fieldSize;
        }while(hiddenFieldArray[rand1][rand2] == MINE || (abs(x - rand1) < 2 && abs(rand2 - y) < 2));

        hiddenFieldArray[rand1][rand2] = MINE;
        assert(hiddenFieldArray[rand1][rand2] == -1);
    }

    short int numberOfMines;
    for (int y = 0; y < fieldSize; y++) {
        for(int x = 0; x < fieldSize; x++) {
            numberOfMines = 0;
            if (hiddenFieldArray[y][x] == MINE) {
                continue;
            }
            qDebug() << x << " " << y << " " << fieldSize << endl;
            if (y != fieldSize - 1) {
                if (hiddenFieldArray[y+1][x] == MINE) { // DOWN
                    numberOfMines++;
                }
            }
            if (y != 0) {
                if (hiddenFieldArray[y-1][x] == MINE) { // UP --
                    numberOfMines++;
                }
            }
            if (x != fieldSize - 1) {
                if (hiddenFieldArray[y][x+1] == MINE) { // RIGHT
                    numberOfMines++;
                }
            }
            if (x != 0) {
                if (hiddenFieldArray[y][x-1] == MINE) { // LEFT--
                    numberOfMines++;
                }
            }
            if (x != fieldSize - 1 && y != 0) {
                if (hiddenFieldArray[y-1][x+1] == MINE) { // UP-RIGHT--
                    numberOfMines++;
                }
            }
            if (x != fieldSize - 1 && y != fieldSize - 1) {
                if (hiddenFieldArray[y+1][x+1] == MINE) { // DOWN-RIGHT
                    numberOfMines++;
                }
            }
            if (x != 0 && y != 0) {
                if (hiddenFieldArray[y-1][x-1] == MINE) { // UP-LEFT--
                    numberOfMines++;
                }
            }
            if (x != 0 && y != fieldSize - 1) {
                if (hiddenFieldArray[y+1][x-1] == MINE) { // DOWN-LEFT--
                    numberOfMines++;
                }
            }

            hiddenFieldArray[y][x] = numberOfMines;
        }

    }
    for (int y = 0; y < fieldSize; y++) {
        for (int x = 0; x < fieldSize; x++) {
            qDebug() << hiddenFieldArray[y][x] << " ";
        }
        qDebug() << endl;
    }
}
