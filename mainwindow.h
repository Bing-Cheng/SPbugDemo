#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#define NUM_THREADS 50
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
QTimer *timer ;
unsigned char* imagebuffers[NUM_THREADS];
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_LoadImage_clicked();

    void on_NextImage_clicked();
    void update();

 void start();
private:
    Ui::MainWindow *ui;

    void process(unsigned char *pImage);
};

#endif // MAINWINDOW_H
