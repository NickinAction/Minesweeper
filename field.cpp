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
    switch (difficulty) {
        case 'e':
            fieldWidth = 10;
            fieldHeight = 10;
            mineCount = 12;
        break;

        case 'm':
            fieldWidth = 25;
            fieldHeight = 25;
            mineCount = 94;
        break;

        case 'h':
            fieldWidth = 30;
            fieldHeight = 38;
            mineCount = 216;
        break;

        default:
            throw std::invalid_argument("No such difficulty mode");
    }

    blockWidth = fieldPixelSize/fieldWidth;
    blockHeight = fieldPixelSize/fieldHeight;

    for(int y = 0; y < fieldHeight; y++) {
        for(int x = 0; x < fieldWidth; x++) {
            fieldArray[y][x] = UNOPENED;
        }
    }


}

void Field::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    auto dp = [&](int y, int x, QPixmap& pm) {
      painter.drawPixmap(blockWidth*x+1, blockHeight*y+1, blockWidth-1, blockHeight-1, pm);
    };

    if (game_status == NOT_STARTED) { // field hasn't been generated yet
        for (int i = 0; i < fieldHeight; i++) {
            for (int j = 0; j < fieldWidth; j++) {
                dp(i,j,unopened_block);
            }
        }
        return;
    }
    for (int y = 0; y < fieldHeight; y++) {
        for (int x = 0; x < fieldWidth; x++) {
            switch(fieldArray[y][x]) {
                case OPENED:
                    dp(y,x,number_images[unsigned(hiddenFieldArray[y][x])]);
                break;
                case FLAG:
                    dp(y,x,flag);
                break;
            default: //unopened
                    dp(y,x,unopened_block);

            }
        }
    }

    for (int y = 0; y < fieldHeight; y++) {
        for (int x = 0; x < fieldWidth; x++) {
            if(hiddenFieldArray[y][x] == MINE) {
                //dp(i,j,mine);
                continue;
            }
            else {
                qDebug() << hiddenFieldArray[y][x] << endl;
                //dp(i,j,number_images[unsigned(hiddenFieldArray[i][j])]);
            }
        }
    }


}

void Field::mousePressEvent(QMouseEvent *e){

    int MEblockX = e->x()/blockWidth; //mouse event block
    int MEblockY = e->y()/blockHeight;

    if (e->button() == Qt::LeftButton) {
        if (withinField(e->x(), e->y())) {

            if(game_status == NOT_STARTED) {
                generateHiddenField(MEblockY, MEblockX);
                game_status = ONGOING;
            }

            if(fieldArray[MEblockY][MEblockX] == UNOPENED) {
                fieldArray[MEblockY][MEblockX] = OPENED;
            }

            if (hiddenFieldArray[MEblockY][MEblockX] == MINE) {

            }

        }

    } else if (e->button() == Qt::RightButton){
        if (withinField(e->x(), e->y())) {

            if(fieldArray[MEblockY][MEblockX] == FLAG) {
                fieldArray[MEblockY][MEblockX] = UNOPENED;
            }
            else if (fieldArray[MEblockY][MEblockX] == UNOPENED) {
                fieldArray[MEblockY][MEblockX] = FLAG;
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
        int rand_y, rand_x;

        do {
            rand_y = rand() % fieldHeight;
            rand_x = rand() % fieldWidth;
        }while(hiddenFieldArray[rand_y][rand_x] == MINE || (abs(x_click - rand_y) < 2 && abs(rand_x - y_click) < 2));

        hiddenFieldArray[rand_y][rand_x] = MINE;
        assert(hiddenFieldArray[rand_y][rand_x] == MINE);
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
