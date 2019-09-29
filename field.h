#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QTimer>

class Field : public QWidget {

//protected:

public:
    explicit Field(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *Event);


    QColor block_color = QColor(170, 170, 170, 255);
    QColor field_color = QColor(187,187,187, 255);


};

#endif // FIELD_H
