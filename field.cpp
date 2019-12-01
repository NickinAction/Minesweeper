#include "field.h"
#include "mainwindow.h"

#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <utility>

#include <QtDebug>
#include <QtWidgets>
#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include <QPixmap>


Field::Field(QWidget *parent, char difficulty) : QWidget(parent){

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
            visibleFieldArray[y][x] = UNOPENED;
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
            switch(visibleFieldArray[y][x]) {
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
                //qDebug() << hiddenFieldArray[y][x] << endl;
                //dp(i,j,number_images[unsigned(hiddenFieldArray[i][j])]);
            }
        }
    }
}

void Field::mousePressEvent(QMouseEvent *e){

    int MEblockX = e->x()/blockWidth; //mouse event block
    int MEblockY = e->y()/blockHeight;


    if (!withinField(e->x(), e->y())) {
        return;
    }

    if (e->button() == Qt::LeftButton) {
        if(game_status == NOT_STARTED) {
            generateHiddenField(MEblockY, MEblockX);
            game_status = ONGOING;
            openFieldSection(MEblockX, MEblockY);
        }
        if(visibleFieldArray[MEblockY][MEblockX] == UNOPENED) {
            visibleFieldArray[MEblockY][MEblockX] = OPENED;
        }
        else if (hiddenFieldArray[MEblockY][MEblockX] == MINE) {
            visibleFieldArray[MEblockY][MEblockX] = OPENED_MINE;
            loseGame();
        }
        else if(visibleFieldArray[MEblockY][MEblockX] == UNOPENED) {
            openFieldSection(MEblockY, MEblockX);
        }
    }

    else if (e->button() == Qt::RightButton){
        if(visibleFieldArray[MEblockY][MEblockX] == FLAG) {
            visibleFieldArray[MEblockY][MEblockX] = UNOPENED;
        }
        else if (visibleFieldArray[MEblockY][MEblockX] == UNOPENED) {
            visibleFieldArray[MEblockY][MEblockX] = FLAG;
        }
    }

    else if (e->button() == Qt::MidButton) {
        if(hiddenFieldArray[MEblockY][MEblockX] == adjacentFlagCount(MEblockX, MEblockY)) {
            openFieldSection(MEblockX, MEblockY, true);
        }
    }

    qDebug() << e->button() << " x: " << e->x() << " y: " << e->y() << endl;
    this->update();
}

short Field::adjacentFlagCount(int cellX, int cellY) {
    short flagCount = 0;
    if (cellY != fieldHeight - 1) {
        if (visibleFieldArray[cellY+1][cellX] == FLAG) { // DOWN
            flagCount++;
        }
    }
    if (cellY != 0) {
        if (visibleFieldArray[cellY-1][cellX] == FLAG) { // UP --
            flagCount++;
        }
    }
    if (cellX != fieldWidth - 1) {
        if (visibleFieldArray[cellY][cellX+1] == FLAG) { // RIGHT
            flagCount++;
        }
    }
    if (cellX != 0) {
        if (visibleFieldArray[cellY][cellX-1] == FLAG) { // LEFT--
            flagCount++;
        }
    }
    if (cellX != fieldWidth - 1 && cellY != 0) {
        if (visibleFieldArray[cellY-1][cellX+1] == FLAG) { // UP-RIGHT--
            flagCount++;
        }
    }
    if (cellX != fieldWidth - 1 && cellY != fieldHeight - 1) {
        if (visibleFieldArray[cellY+1][cellX+1] == FLAG) { // DOWN-RIGHT
            flagCount++;
        }
    }
    if (cellX != 0 && cellY != 0) {
        if (visibleFieldArray[cellY-1][cellX-1] == FLAG) { // UP-LEFT--
            flagCount++;
        }
    }
    if (cellX != 0 && cellY != fieldHeight - 1) {
        if (visibleFieldArray[cellY+1][cellX-1] == FLAG) { // DOWN-LEFT--
            flagCount++;
        }
    }

    return flagCount;
}

void Field::loseGame() {
    game_status = LOST;
}

void Field::openFieldSection(int start_x, int start_y, bool middleClick) { //wave agl
    class Point {
    public:
        int x;
        int y;
        Point(int x, int y) {
            this->x = x;
            this->y = y;
        }
    };

    using std::queue;

    queue <Point> cells;

    cells.push(Point(start_x,start_y));

    bool ignoreIfNumber = middleClick;

    while(!cells.empty()) {
        Point cur_point = cells.front();
        int x = cur_point.x;
        int y = cur_point.y;
        cells.pop();


        if (visibleFieldArray[y][x] == FLAG) {
            continue;
        }
        else if (hiddenFieldArray[y][x] == MINE && middleClick) {
            loseGame();
            return;
        }
        else if (hiddenFieldArray[y][x] == MINE) {
            throw std::runtime_error("Found a mine while block opening");
        }
        if(visibleFieldArray[y][x] == OPENED || (hiddenFieldArray[y][x] != 0)) {
            visibleFieldArray[y][x] = OPENED;
            if (ignoreIfNumber) {

                ignoreIfNumber = false;
            } else {
                continue;
            }
        }

        if (y != fieldHeight - 1) { // DOWN
            cells.push(Point(x, y + 1));
        }
        if (y != 0) { // UP
            cells.push(Point(x, y - 1));
        }
        if (x != fieldWidth - 1) { // RIGHT
            cells.push(Point(x + 1, y));
        }
        if (x != 0) { // LEFT
            cells.push(Point(x - 1, y));
        }
        if (x != fieldWidth - 1 && y != 0) { // UP-RIGHT
            cells.push(Point(x + 1, y - 1));
        }
        if (x != fieldWidth - 1 && y != fieldHeight - 1) { // DOWN-RIGHT
            cells.push(Point(x + 1, y + 1));
        }
        if (x != 0 && y != 0) {// UP-LEFT
            cells.push(Point(x - 1, y - 1));
        }
        if (x != 0 && y != fieldHeight - 1) { // DOWN-LEFT
            cells.push(Point(x - 1, y + 1));
        }

        visibleFieldArray[y][x] = OPENED;
    }

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
}
