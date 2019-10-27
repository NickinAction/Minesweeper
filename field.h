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
    void paintEvent(QPaintEvent *Event);
    // paintEvent overrides a standard function. Better to use 'override' tag to note that.
    //void Field::mousePressEvent(QMouseEvent *event);

    QColor block_color = QColor(170, 170, 170, 255);
    QColor field_color = QColor(187,187, 187, 255);

    int fieldPixelSize = 950; // Is it a constant? If so, make it a constant.
    int blockSize;

    char difficulty; //can be 'e' for easy, 'm' for medium and 'h' for hard.
    // Think about making it an enumerator then. This way you can limit the possible values to
    // these three exclusively.
    int fieldSize;
};

#endif // FIELD_H
