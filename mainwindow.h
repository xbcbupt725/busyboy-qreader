#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>

#include "qxtglobalshortcut/qxtglobalshortcut.h"
#include "QZXing.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QLabel *label;
    QLabel *infoLabel;
    bool isOnSnapping;
    QPixmap fullScreen;
    QString helpMsg;
    QString iMsg;

    QPoint start;
    QPoint end;
    bool isLeftButtonPressed;
    QRubberBand *rubberBand;

    QClipboard *clipboard;

    void keyPressEvent(QKeyEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private slots:
    void onSnap();
    void onQr();
    void onSave();
};

#endif // MAINWINDOW_H
