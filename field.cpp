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

    switch (difficulty) {
        case 'e':
            //this->resize(500, 500);
            fieldHeightInCells = 9;
            fieldWidthInCells = 9;
            mineCount = 10;
        break;

        case 'm':
            fieldHeightInCells = 16;
            fieldWidthInCells = 16;
            mineCount = 40;
        break;

        case 'h':
            fieldHeightInCells = 16;
            fieldWidthInCells = 30;
            mineCount = 99;
        break;

        default:
            throw std::invalid_argument("No such difficulty mode");
    }

    flagCount = mineCount;

    if (fieldHeightInCells > fieldWidthInCells) {
        fieldPixelWidth = fieldPixelHeight * fieldWidthInCells/ fieldHeightInCells;
    }
    else if (fieldWidthInCells > fieldHeightInCells) {
        fieldPixelHeight = fieldPixelWidth * fieldHeightInCells/ fieldWidthInCells;
    }
    // there shouldn't be overflow with current numbers.
    // Still, be careful with this line.

    blockPixelWidth = fieldPixelWidth/fieldWidthInCells;
    blockPixelHeight = fieldPixelHeight/fieldHeightInCells;

    // update fieldPixelWidth&Height to remove rounding error & maintain correct border-click processing
    fieldPixelWidth = blockPixelWidth * fieldWidthInCells;
    fieldPixelHeight = blockPixelHeight * fieldHeightInCells;

    emit resizeInLayout(fieldPixelWidth, fieldPixelHeight);

    for(int y = 0; y < fieldHeightInCells; y++) {
        for(int x = 0; x < fieldWidthInCells; x++) {
            visibleFieldArray[y][x] = UNOPENED;
        }
    }

}


void Field::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    auto dp = [&](int y, int x, QPixmap& pm) {
      painter.drawPixmap(blockPixelWidth*x+1, blockPixelHeight*y+1, blockPixelWidth-1, blockPixelHeight-1, pm);
    };

    for (int y = 0; y < fieldHeightInCells; y++) {
        for (int x = 0; x < fieldWidthInCells; x++) {
            switch(visibleFieldArray[y][x]) {
                case DETONATED_MINE:
                    painter.fillRect(blockPixelWidth*x+1, blockPixelHeight*y+1, blockPixelWidth-1, blockPixelHeight, Qt::red);
                    dp(y,x,mine);
                break;
                case OPENED_MINE:
                    dp(y,x, number_images[0]);
                    dp(y,x,mine);
                break;
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
    /* code used for testing ( draws the whole field )
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
    }*/
}

void Field::mousePressEvent(QMouseEvent *e){


    if(game_status == WON || game_status == LOST) return;

    int MEblockX = e->x()/blockPixelWidth; //mouse event block
    int MEblockY = e->y()/blockPixelHeight;


    if (!withinField(e->x(), e->y())) {
        return;
    }

    if (e->button() == Qt::LeftButton) {
        if(game_status == NOT_STARTED) {
            generateHiddenField(MEblockY, MEblockX);
            if (visibleFieldArray[MEblockY][MEblockX] != FLAG) {
                game_status = ONGOING;
                emit startGameTimer();
                openFieldSection(MEblockX, MEblockY);
            }
        }
        if (hiddenFieldArray[MEblockY][MEblockX] == MINE) {
            visibleFieldArray[MEblockY][MEblockX] = DETONATED_MINE;
            loseGame();
            emit updateGameStatus(LOST);

        }
        else if(visibleFieldArray[MEblockY][MEblockX] == UNOPENED) {
            openFieldSection(MEblockX, MEblockY);
        }
    }

    else if (e->button() == Qt::RightButton){
        if(visibleFieldArray[MEblockY][MEblockX] == FLAG) {
            visibleFieldArray[MEblockY][MEblockX] = UNOPENED;
            flagCount++;
            emit sendFlagCount(flagCount);
        }
        else if (visibleFieldArray[MEblockY][MEblockX] == UNOPENED) {
            visibleFieldArray[MEblockY][MEblockX] = FLAG;
            flagCount--;
            emit sendFlagCount(flagCount);
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
    if (cellY != fieldHeightInCells - 1) {
        if (visibleFieldArray[cellY+1][cellX] == FLAG) { // DOWN
            flagCount++;
        }
    }
    if (cellY != 0) {
        if (visibleFieldArray[cellY-1][cellX] == FLAG) { // UP --
            flagCount++;
        }
    }
    if (cellX != fieldWidthInCells - 1) {
        if (visibleFieldArray[cellY][cellX+1] == FLAG) { // RIGHT
            flagCount++;
        }
    }
    if (cellX != 0) {
        if (visibleFieldArray[cellY][cellX-1] == FLAG) { // LEFT--
            flagCount++;
        }
    }
    if (cellX != fieldWidthInCells - 1 && cellY != 0) {
        if (visibleFieldArray[cellY-1][cellX+1] == FLAG) { // UP-RIGHT--
            flagCount++;
        }
    }
    if (cellX != fieldWidthInCells - 1 && cellY != fieldHeightInCells - 1) {
        if (visibleFieldArray[cellY+1][cellX+1] == FLAG) { // DOWN-RIGHT
            flagCount++;
        }
    }
    if (cellX != 0 && cellY != 0) {
        if (visibleFieldArray[cellY-1][cellX-1] == FLAG) { // UP-LEFT--
            flagCount++;
        }
    }
    if (cellX != 0 && cellY != fieldHeightInCells - 1) {
        if (visibleFieldArray[cellY+1][cellX-1] == FLAG) { // DOWN-LEFT--
            flagCount++;
        }
    }

    return flagCount;
}

void Field::loseGame() {
    game_status = LOST;

    // open the field
    for(int y = 0; y < fieldHeightInCells; y++) {
        for(int x = 0; x < fieldWidthInCells; x++) {
            if (visibleFieldArray[y][x] == DETONATED_MINE) {
                continue;
            }
            else if(hiddenFieldArray[y][x] == MINE) {
                visibleFieldArray[y][x] = OPENED_MINE;
            }
            else visibleFieldArray[y][x] = OPENED;
        }
    }
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
            visibleFieldArray[y][x] = DETONATED_MINE;
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

        if (y != fieldHeightInCells - 1) { // DOWN
            cells.push(Point(x, y + 1));
        }
        if (y != 0) { // UP
            cells.push(Point(x, y - 1));
        }
        if (x != fieldWidthInCells - 1) { // RIGHT
            cells.push(Point(x + 1, y));
        }
        if (x != 0) { // LEFT
            cells.push(Point(x - 1, y));
        }
        if (x != fieldWidthInCells - 1 && y != 0) { // UP-RIGHT
            cells.push(Point(x + 1, y - 1));
        }
        if (x != fieldWidthInCells - 1 && y != fieldHeightInCells - 1) { // DOWN-RIGHT
            cells.push(Point(x + 1, y + 1));
        }
        if (x != 0 && y != 0) {// UP-LEFT
            cells.push(Point(x - 1, y - 1));
        }
        if (x != 0 && y != fieldHeightInCells - 1) { // DOWN-LEFT
            cells.push(Point(x - 1, y + 1));
        }

        visibleFieldArray[y][x] = OPENED;
    }

   if (checkWin()) winGame();

}

bool Field::checkWin() {
    for (int x = 0; x < fieldWidthInCells; x++) {
        for (int y = 0; y < fieldHeightInCells; y++) {
            if (visibleFieldArray[x][y] != OPENED && hiddenFieldArray[x][y] != MINE) {
                return false;
            }
        }
    }
    return true;
}

void Field::winGame() {
    game_status = WON;

    emit updateGameStatus(WON);

}

bool Field::withinField(int x, int y) {
    return(x >= 0 && x < fieldPixelWidth && y >= 0 && y < fieldPixelHeight);
}

void Field::generateHiddenField(int x_click, int y_click) {
    srand (unsigned(time(nullptr)));

    for (int y = 0; y < fieldHeightInCells; y ++) {
        for (int x = 0; x < fieldWidthInCells; x++) {
            hiddenFieldArray[y][x] = 0;
        }
    }
    for(int i = 0; i < mineCount; i++) {
        int rand_y, rand_x;

        do {
            rand_y = rand() % fieldHeightInCells;
            rand_x = rand() % fieldWidthInCells;
        }while(hiddenFieldArray[rand_y][rand_x] == MINE || (abs(x_click - rand_y) < 2 && abs(rand_x - y_click) < 2));

        hiddenFieldArray[rand_y][rand_x] = MINE;
        assert(hiddenFieldArray[rand_y][rand_x] == MINE);
    }

    short int numberOfMines;
    for (int y = 0; y < fieldHeightInCells; y++) {
        for(int x = 0; x < fieldWidthInCells; x++) {
            numberOfMines = 0;
            if (hiddenFieldArray[y][x] == MINE) {
                continue;
            }
            if (y != fieldHeightInCells - 1) {
                if (hiddenFieldArray[y+1][x] == MINE) { // DOWN
                    numberOfMines++;
                }
            }
            if (y != 0) {
                if (hiddenFieldArray[y-1][x] == MINE) { // UP --
                    numberOfMines++;
                }
            }
            if (x != fieldWidthInCells - 1) {
                if (hiddenFieldArray[y][x+1] == MINE) { // RIGHT
                    numberOfMines++;
                }
            }
            if (x != 0) {
                if (hiddenFieldArray[y][x-1] == MINE) { // LEFT--
                    numberOfMines++;
                }
            }
            if (x != fieldWidthInCells - 1 && y != 0) {
                if (hiddenFieldArray[y-1][x+1] == MINE) { // UP-RIGHT--
                    numberOfMines++;
                }
            }
            if (x != fieldWidthInCells - 1 && y != fieldHeightInCells - 1) {
                if (hiddenFieldArray[y+1][x+1] == MINE) { // DOWN-RIGHT
                    numberOfMines++;
                }
            }
            if (x != 0 && y != 0) {
                if (hiddenFieldArray[y-1][x-1] == MINE) { // UP-LEFT--
                    numberOfMines++;
                }
            }
            if (x != 0 && y != fieldHeightInCells - 1) {
                if (hiddenFieldArray[y+1][x-1] == MINE) { // DOWN-LEFT--
                    numberOfMines++;
                }
            }

            hiddenFieldArray[y][x] = numberOfMines;
        }

    }
}

int Field::getFlagCount() {
    return flagCount;
}
