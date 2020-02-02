#ifndef FIELD_H
#define FIELD_H

#include <QTime>
#include <QTimer>
#include <QObject>
#include <QWidget>

class Field : public QWidget {
    Q_OBJECT

public:

    explicit Field(QWidget *parent = nullptr, char difficulty = 'e');
    int getFlagCount();

signals:
    void sendFlagCount(int flagCount);
    void startGameTimer(bool fromField = true);
    void updateGameStatus(char newStatus);
    void resizeInLayout(int newWidth, int newHeight);
    void toggleSmile(bool shocked);
private:

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *Event) override;

    int fieldPixelWidth = 950;
    int fieldPixelHeight = 950;
    int blockPixelWidth, blockPixelHeight;
    int highlightX = -1, highlightY = -1;
    // initializing higlights with impossible values so
    // moveEvent always updates on its first call
    int fieldWidthInCells, fieldHeightInCells;
    int mineCount;
    int flagCount;
    bool drawflag = false;
    bool highlightLeft = false, highlightMiddle = false;
    char visibleFieldArray[38][38]; // maximum size that we may need.
    short int hiddenFieldArray[38][38];

    enum VISIBLE_FIELD_VALUES {
        UNOPENED = 'u',
        FLAG = 'f',
        OPENED = 'o',
        OPENED_MINE = 'm', // only in use when the game is lost
        DETONATED_MINE = 'd' // only in use when the game is lost
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
    bool checkWin();
    void winGame();

};

#endif // FIELD_H
