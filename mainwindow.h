#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
     QImage imageObject;
int threadNumber;
int imageCnt;
unsigned char* imagebuffers[5];
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_LoadImage_clicked();

    void on_NextImage_clicked();

 void start();
private:
    Ui::MainWindow *ui;

    void process(unsigned char *pImage);
};

#endif // MAINWINDOW_H
