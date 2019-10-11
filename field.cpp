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

Field::Field(QWidget *parent, char difficulty) : QWidget(parent){
    this->difficulty = difficulty;
}

// It is possible to suppress the warning you need to ignore so it won't distract.
// Like this:
// void Field::paintEvent(__attribute__((unused))QPaintEvent *Event) {
void Field::paintEvent(QPaintEvent *Event) {

    QPainter painter(this);
    QPixmap flag = QPixmap(":/new/images/flag.png");
    // And yes, rename the folder pls.
    //painter.drawPixmap(10,10,50,50, flag);

    painter.fillRect(0, 0, fieldPixelSize, fieldPixelSize, block_color);

    //difficulty = 'h';//temporary
    // You don't need this one anymore, so you can remove it.
    // In general, it's better not to leave code on comment for a long time.
    // You can always retrieve it back from Git. That's what it is for.
    // Same applies to code comments above and below.

    // This code just asksfor switch/case instead.
    /// Also, why do you find the difficulty and the block size EVERY time we call paintEvent?
    /// It's not like it can change any time.
    /// Better to find this value in the class constructor.
    /// Or even in a separate function that is called from the constructor, if you prefer.
    if (difficulty == 'e'){
        fieldSize = 10;
    }
    else if (difficulty == 'm') {
        fieldSize = 25;
    }
    else if (difficulty == 'h') {
        fieldSize = 38;
    }

    blockSize = fieldPixelSize/fieldSize;

    for (int i = 0; i < fieldSize; i++) {
        for (int j = 0; j < fieldSize; j++) {
            painter.drawRect(blockSize*i, blockSize*j, blockSize, blockSize);
        }
    }

    painter.drawPixmap(0, 0, blockSize, blockSize, flag);
    //QPoint pos = QCursor::pos();
}

void Field::mousePressEvent(QMouseEvent *e){
    qDebug() << e->button() << " x: " << e->x() << " y: " << e->y() << endl;
}

