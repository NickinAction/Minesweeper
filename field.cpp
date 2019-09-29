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

Field::Field(QWidget *parent) : QWidget(parent)
{
    QColor block_color = QColor(155, 170, 170, 255);
    QColor field_color = QColor(187,187,187, 255);

}

void Field::paintEvent(QPaintEvent *Event) {

    QPainter painter(this);
    painter.fillRect(10, 10, 950, 950, block_color);

}
