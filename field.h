#ifndef FIELD_H
#define FIELD_H

#include <QTime>
#include <QTimer>
#include <QObject>
#include <QWidget>

class Field : public QWidget {

public:

    explicit Field(QWidget *parent = nullptr, char difficulty = 'e');

private:

    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *Event) override;

    const int fieldPixelSize = 950; //the pixel size of the entire field
    // TODO: allow the whole field to be non-square, too.

    int blockWidth; // the size of one block on the field
    int blockHeight;

    int fieldWidth;
    int fieldHeight;
    int mineCount;
    int flagCount;
    bool drawflag = false;
    char visibleFieldArray[38][38]; // maximum size that we may need.
    short int hiddenFieldArray[38][38];

    enum VISIBLE_FIELD_VALUES {
        UNOPENED = 'u',
        FLAG = 'f',
        OPENED = 'o',
        OPENED_MINE = 'm',
        DETONATED_MINE = 'd'
    };

    char game_status = 'n';
    enum GAME_STATUS {
        NOT_STARTED = 'n',
        ONGOING = 'o',
        PAUSED = 'p',
        LOST = 'l',
        WON = 'w'
    };

    const short int MINE = -1;

    QPixmap flag;
    QPixmap unopened_block;
    QPixmap mine;
    std::vector <QPixmap> number_images;

    bool withinField(int x, int y);
    void generateHiddenField(int x, int y);
    void openFieldSection(int x, int y, bool middleClick = false);
    short adjacentFlagCount(int cellX, int cellY);

    void loseGame();

};

#endif // FIELD_H
