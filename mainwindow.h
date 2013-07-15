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
     QImage imageObjectOut;
    int threadNumber;
    int imagePre;
    int imageCnt;
    QTimer *timer ;
    QTimer *timer2 ;
    int slideStep;
    int threadDelay;
    bool lockMutex;
    static pthread_mutex_t* mutexsum;
    unsigned char* imagebuffers[NUM_THREADS];
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_clicked();
    void on_LoadImage_clicked();
    void on_NextImage_clicked();
    void update();
    void slideImage();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_pressed();

private:
    Ui::MainWindow *ui;
    void process(int delay);
};

#endif // MAINWINDOW_H
