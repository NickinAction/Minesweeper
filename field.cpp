#include "field.h"
#include <QTime>
#include <QTimer>
#include <QtWidgets>
#include "mainwindow.h"
#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <QtDebug>
#include <QPainter>

Field::Field(QWidget *parent) : QWidget(parent){

}

void Field::paintEvent(QPaintEvent *Event) {

    QPainter painter(this);
    painter.fillRect(10, 10, 950, 950, block_color);

}
