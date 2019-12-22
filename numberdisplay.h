#ifndef NUMBERDISPLAY_H
#define NUMBERDISPLAY_H

#include <QWidget>

class NumberDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit NumberDisplay(QWidget *parent = nullptr);

signals:

public slots:

private:
    void paintEvent(QPaintEvent *Event) override;
    int number = 0;
    const int blocks = 3;


    std::vector <QPixmap> counter_images;
    QPixmap minus_sign;

};

#endif // NUMBERDISPLAY_H
