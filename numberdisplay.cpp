#include "numberdisplay.h"
#include <QPainter>

NumberDisplay::NumberDisplay(QWidget *parent) : QWidget(parent)
{
    for (int i = 0; i <= 9; i++) {
        std::string number_images_address = ":/images/timer" + std::string(1, char('0' + i)) + ".png";
        counter_images.push_back(QPixmap(QString::fromStdString(number_images_address)));
    }

    minus_sign = QPixmap(":/images/timer_minus");

}

void NumberDisplay::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    int w = this->width()/blocks;
    int h = this->height();

    auto dp = [&](int x, QPixmap& pm) {
        painter.drawPixmap(x * w, 0, w, h, pm);
    };
    //number; // 365, 0, 1, -15

    int ones;
    int tens;
    int hundreds;

    ones = abs(number)%10;
    tens = (abs(number)/10)%10;
    hundreds =(abs(number)/100)%10;

    dp(2, counter_images[unsigned(ones)]);
    dp(1, counter_images[unsigned(tens)]);

    if(number >= 0) {
        dp(0, counter_images[unsigned(hundreds)]);
    }
    else {
        dp(0, minus_sign);
    }
}


void NumberDisplay::setFlagNum(int number) {
    this->number = number;
    update();
}
