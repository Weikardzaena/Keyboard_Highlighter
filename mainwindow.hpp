#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include<Windows.h>
#include<QMessageBox>
#include<QKeyEvent>
#include<QMainWindow>
#include<QLibrary>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsPixmapItem>
#include<QPixmap>
#include<QTimer>
#include<vector>
#include<fstream>
#include<bitset>

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit    mainWindow(QWidget *parent = 0);
    void        beginLoop();

    bool        initFailed;

public slots:
    void        updateInputState();

private:
    void        initScene(QRect *rect);
    bool        initVars();
    void        initView(QRect *rect);
    bool        loadPixmaps();
    bool        loadPixmapPosData();
    void        keyPressEvent(QKeyEvent *event);

    QPixmap             *keyboardBackground;
    QPixmap             *backspaceKeyPixmap;
    QPixmap             *bottomRowKeyPixmap;
    QPixmap             *capsKeyPixmap;
    QPixmap             *enterKeyPixmap;
    QPixmap             *pipeKeyPixmap;
    QPixmap             *plusKeyRightPixmap;
    QPixmap             *shiftKeyLeftPixmap;
    QPixmap             *shiftKeyRightPixmap;
    QPixmap             *spaceKeyPixmap;
    QPixmap             *squareKeyPixmap;
    QPixmap             *tabKeyPixmap;
    QPixmap             *zeroKeyPixmap;
    QGraphicsPixmapItem *keyHighlights[254];
    QGraphicsScene      *keyboardScene;
    QGraphicsView       *keyboardView;
    QTimer              *timer;
    QLibrary            user32;
    std::vector<float>  pixmapPosData;
    std::bitset<256>    keyboardArray;
};
#endif // MAINWINDOW_HPP
