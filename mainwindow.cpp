#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowTitle("BusyBoy.snap with qr[按下H键显示帮助]");
    setMinimumSize(360, 80);
    setWindowIcon(QIcon(":images/icon"));
    clipboard = QApplication::clipboard();
    QxtGlobalShortcut *sc_z = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+Z"), this);
    connect(sc_z, SIGNAL(activated()),
            this, SLOT(onSnap()));
    QxtGlobalShortcut *sc_x = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+X"), this);
    connect(sc_x, SIGNAL(activated()),
            this, SLOT(onQr()));
    QxtGlobalShortcut *sc_s = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+S"), this);
    connect(sc_s, SIGNAL(activated()),
            this, SLOT(onSave()));

    label = new QLabel(this);
    infoLabel = new QLabel(this);
    helpMsg.append("按下Ctrl+Alt+Z截图\n");
    helpMsg.append("按下Ctrl+Alt+X识别二维码\n");
    helpMsg.append("按下Ctrl+Alt+S保存图片\n");
    helpMsg.append("按下h显示本帮助\n");
    helpMsg.append("Esc退出本程序\n");
    infoLabel->setText(helpMsg);
    infoLabel->adjustSize();
    isOnSnapping = false;
    rubberBand = new QRubberBand(QRubberBand::Rectangle, label);
    isLeftButtonPressed = false;

    iMsg.append("原谅我这一生不羁放纵爱自由!");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *ev){
    if(ev->key() == Qt::Key_Escape){
        if(isOnSnapping == true){
            this->showNormal();
            isOnSnapping = false;
        }else if(isOnSnapping == false){
            QApplication::exit(0);
        }
    }else if(ev->key() == Qt::Key_H){
        this->showNormal();
        label->setText(helpMsg);
        label->adjustSize();
        infoLabel->hide();
        this->adjustSize();
    }else if(ev->key() == Qt::Key_I){
        QMessageBox::about(this,"i", iMsg);
    }
}

void MainWindow::onSnap(){
    fullScreen = QPixmap::grabWindow(QApplication::desktop()->winId());
    this->showNormal();
    this->showFullScreen();
    label->setPixmap(fullScreen);
    label->show();
    label->adjustSize();
    isOnSnapping = true;

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev){
    QPoint pos = ev->pos();

    // 截图状态显示截图信息
    if(isOnSnapping){
        int x = pos.x();
        int y = pos.y();
        int w = infoLabel->width();
        int h = infoLabel->height();

        QString info = "";
        info.append("x:").append(QString::number(start.x()));
        info.append("\ny:").append(QString::number(start.y()));
        info.append("\n*\nwidth:").append(QString::number(pos.x() - start.x()));
        info.append("\nheight:").append(QString::number(pos.y() - start.y()));

        infoLabel->setText(info);
        infoLabel->setGeometry(QRect(x, y, w, h));
        infoLabel->adjustSize();
        infoLabel->setStyleSheet("background:#eaeaea;");
        infoLabel->show();
    }else{
        infoLabel->hide();
    }
    if(ev->type() == QEvent::MouseMove&&
            isLeftButtonPressed){
        if(rubberBand){
            rubberBand->setGeometry(QRect(start, ev->pos()).normalized());
            rubberBand->show();
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev){
    if(ev->button() == Qt::LeftButton&&
            ev->type() == QEvent::MouseButtonPress&&
            isOnSnapping){
        isLeftButtonPressed = true;
        start = ev->pos();
        if(!rubberBand){
            rubberBand = new QRubberBand(QRubberBand::Rectangle,
                                         label);
        }
        rubberBand->setGeometry(QRect(start, QSize()));
        rubberBand->show();
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *ev){
    if(isLeftButtonPressed&&
            isOnSnapping){
        isLeftButtonPressed = false;
        end = ev->pos();
        QPixmap pixmap = fullScreen.copy(QRect(start, end));
        clipboard->setImage(pixmap.toImage());
        this->showNormal();
        label->setPixmap(pixmap);
        label->adjustSize();
        this->resize(label->size());
        rubberBand->hide();
        isOnSnapping = false;
    }
}

void MainWindow::onQr(){
    QPixmap pixmap = fullScreen.copy(QRect(start, end));
    if(pixmap.isNull()){
        QMessageBox::warning(this, "提示", "还未截图");
    }else{
        QZXing decoder;
        QString qrmsg = decoder.decodeImage(pixmap.toImage());
        if(qrmsg.isEmpty()){
            QMessageBox::warning(this, "识别失败", "请截取完整的二维码");
        }else{
            QMessageBox::information(this, "识别成功-已经复制到剪切板",qrmsg);
            clipboard->setText(qrmsg);
        }
    }
}

void MainWindow::onSave(){
    QPixmap pixmap = fullScreen.copy(QRect(start, end));
    if(pixmap.isNull()){
        QMessageBox::warning(this, "提示", "还未截图");
    }else{
      QString savePath = QFileDialog::getSaveFileName(this,
                                                tr("Open Image"),
                                                ".",
                                                tr("Image Files(*.jpg,*.png,*.bmp)"));
      if(savePath.endsWith(".png") ||
              savePath.endsWith(".jpg") ||
              savePath.endsWith(".bmp")){

      }else{
          if(QFileInfo(savePath).fileName().isEmpty()){
              savePath += "image";
          }
          savePath.append(".png");
      }
      QFileInfo finfo(savePath);
      if(finfo.exists()){
          QMessageBox::StandardButton reply =
                  QMessageBox::question(this,"提示","["+finfo.fileName()+"]文件已经存在是否要覆盖?",
                                        QMessageBox::Yes |
                                        QMessageBox::No);
          if(reply == QMessageBox::No){
              return;
          }
      }
      pixmap.save(savePath);
    }
}
