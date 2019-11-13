#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QTimer>

class Field : public QWidget {

public:
    explicit Field(QWidget *parent = nullptr, char difficulty = 'e');
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *Event) override;

    QColor block_color = QColor(170, 170, 170, 255); // color of the blocks on the field
    QColor field_color = QColor(187,187, 187, 255); // color of the field

    const int fieldPixelSize = 950; //the pixel size of the entire field
    int blockSize; // the size of one block on the field

    char difficulty; //can be 'e' for easy, 'm' for medium and 'h' for hard.
    // Think about making it an enumerator then. This way you can limit the possible values to
    // these three exclusively.
    int fieldSize;
    bool drawflag = false;
    char fieldArray[38][38];
    short int hiddenFieldArray[38][38];

private:

    enum FIELD_VALUES {
        UNOPENED = 'u',
        FLAG = 'f',
        OPENED = 'o'
    };
    const short int MINE = -1;

    QPixmap flag;
    QPixmap unopened_block;
    QPixmap mine;
    QPixmap opened_block;
    QPixmap number1;
    QPixmap number2;
    QPixmap number3;
    QPixmap number4;
    QPixmap number5;
    QPixmap number6;
    QPixmap number7;
    QPixmap number8;


    int mineCount;
    bool firstClick = true;

    bool withinField(int x, int y);
    void generateHiddenField(int x, int y);
    void openFieldSection(int x, int y);

};

#endif // FIELD_H
