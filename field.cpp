  #include "field.h"
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
        fieldWidth = 10;
        fieldHeight = 10;
        mineCount = 12;
    }
    else if (difficulty == 'm') {
        fieldWidth = 25;
        fieldHeight = 25;
        mineCount = 94;
    }
    else if (difficulty == 'h') {
        fieldWidth = 38;
        fieldHeight = 38;
        mineCount = 216;
    }

    blockWidth = fieldPixelSize/fieldWidth;
    blockHeight = fieldPixelSize/fieldHeight;

    for(int i = 0; i < fieldHeight; i++) {
        for(int j = 0; j < fieldWidth; j++) {
            fieldArray[i][j] = UNOPENED;
        }
    }


}

void Field::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    auto dp = [&](int i, int j, QPixmap& pm) {
      painter.drawPixmap(blockWidth*i+1, blockHeight*j+1, blockWidth-1, blockHeight-1, pm);
    };

    if (game_status == NOT_STARTED) { // field hasn't been generated yet
        for (int i = 0; i < fieldHeight; i++) {
            for (int j = 0; j < fieldWidth; j++) {
                dp(i,j,unopened_block);
            }
        }
        return;
    }
    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            switch(fieldArray[i][j]) {
                case OPENED:
                    dp(i,j,number_images[unsigned(hiddenFieldArray[i][j])]);
                break;
                case FLAG:
                    dp(i,j,flag);
                break;
            default: //unopened
                    dp(i,j,unopened_block);

            }
        }
    }

    for (int i = 0; i < fieldHeight; i++) {
        for (int j = 0; j < fieldWidth; j++) {
            if(hiddenFieldArray[i][j] == MINE) {
                dp(i,j,mine);
                continue;
            }
            else {
                qDebug() << hiddenFieldArray[i][j] << endl;
                dp(i,j,number_images[unsigned(hiddenFieldArray[i][j])]);
            }
        }
    }


}

void Field::mousePressEvent(QMouseEvent *e){

    int MEblockX; // mouse event block
    int MEblockY;

    if (e->button() == Qt::LeftButton) {
        if (withinField(e->x(), e->y())) {
            MEblockX = e->x()/blockWidth;
            MEblockY = e->y()/blockHeight;

            if(game_status == NOT_STARTED) {
                generateHiddenField(MEblockX, MEblockY);
                game_status = ONGOING;
            }

            if(fieldArray[MEblockX][MEblockY] == UNOPENED) {
                fieldArray[MEblockX][MEblockY] = OPENED;
            }

            if (hiddenFieldArray[MEblockX][MEblockY] == MINE) {

            }

        }

    } else if (e->button() == Qt::RightButton){
        if (withinField(e->x(), e->y())) {
            MEblockX = e->x()/blockWidth;
            MEblockY = e->y()/blockHeight;

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
    ///TODO
}


bool Field::withinField(int x, int y) {
    return(x >= 0 && x <= fieldPixelSize && x >= 0 && y <= fieldPixelSize);
}

void Field::generateHiddenField(int x_click, int y_click) {
    srand (unsigned(time(nullptr)));

    for (int y = 0; y < fieldHeight; y ++) {
        for (int x = 0; x < fieldWidth; x++) {
            hiddenFieldArray[y][x] = 0;
        }
    }
    for(int i = 0; i < mineCount; i++) {
        int rand1, rand2;

        do {
            rand1 = rand() % fieldWidth;
            rand2 = rand() % fieldHeight;
        }while(hiddenFieldArray[rand1][rand2] == MINE || (abs(x_click - rand1) < 2 && abs(rand2 - y_click) < 2));

        hiddenFieldArray[rand1][rand2] = MINE;
        assert(hiddenFieldArray[rand1][rand2] == MINE);
    }

    short int numberOfMines;
    for (int y = 0; y < fieldHeight; y++) {
        for(int x = 0; x < fieldWidth; x++) {
            numberOfMines = 0;
            if (hiddenFieldArray[y][x] == MINE) {
                continue;
            }
            if (y != fieldHeight - 1) {
                if (hiddenFieldArray[y+1][x] == MINE) { // DOWN
                    numberOfMines++;
                }
            }
            if (y != 0) {
                if (hiddenFieldArray[y-1][x] == MINE) { // UP --
                    numberOfMines++;
                }
            }
            if (x != fieldWidth - 1) {
                if (hiddenFieldArray[y][x+1] == MINE) { // RIGHT
                    numberOfMines++;
                }
            }
            if (x != 0) {
                if (hiddenFieldArray[y][x-1] == MINE) { // LEFT--
                    numberOfMines++;
                }
            }
            if (x != fieldWidth - 1 && y != 0) {
                if (hiddenFieldArray[y-1][x+1] == MINE) { // UP-RIGHT--
                    numberOfMines++;
                }
            }
            if (x != fieldWidth - 1 && y != fieldHeight - 1) {
                if (hiddenFieldArray[y+1][x+1] == MINE) { // DOWN-RIGHT
                    numberOfMines++;
                }
            }
            if (x != 0 && y != 0) {
                if (hiddenFieldArray[y-1][x-1] == MINE) { // UP-LEFT--
                    numberOfMines++;
                }
            }
            if (x != 0 && y != fieldHeight - 1) {
                if (hiddenFieldArray[y+1][x-1] == MINE) { // DOWN-LEFT--
                    numberOfMines++;
                }
            }

            hiddenFieldArray[y][x] = numberOfMines;
        }

    }
    for (int y = 0; y < fieldHeight; y++) {
        for (int x = 0; x < fieldWidth; x++) {
            qDebug() << hiddenFieldArray[y][x] << " ";
        }
        qDebug() << endl;
    }
}
