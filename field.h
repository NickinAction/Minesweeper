#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QTimer>

class Field : public QWidget {

public:
    explicit Field(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *Event);
    //void Field::mousePressEvent(QMouseEvent *event);

    QColor block_color = QColor(170, 170, 170, 255);
    QColor field_color = QColor(187,187, 187, 255);


    int fieldPixelSize = 950;
    int blockSize;

    char difficulty; //can be 'e' for easy, 'm' for medium and 'h' for hard.
    int fieldSize;

};

#endif // FIELD_H
