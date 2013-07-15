#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <mythread.h>

#include <iostream>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sem.h>
#include </home/bcheng/workspacecpp/tcmake/test2/src/IsEnhance.h>
#include <fstream>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



using namespace std;
pthread_mutex_t* MainWindow::mutexsum = new pthread_mutex_t();
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    slideStep = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(slideImage()));
    imagePre = 0;
    imageCnt = 0;
    ui->setupUi(this);
    ui->delayms->setText("Delay 0 ms");
    lockMutex = ui->lockBox->isChecked();
    QImage image;
    image.load("/home/bcheng/tt4.jpg");
    imageObject= image.convertToFormat(QImage::Format_RGB888, Qt::AutoColor);
    imageObjectOut= image.convertToFormat(QImage::Format_RGB888, Qt::AutoColor);
    ui->label->setPixmap(QPixmap::fromImage(imageObjectOut));
    pthread_mutex_init(MainWindow::mutexsum, NULL);
}

MainWindow::~MainWindow()
{
    pthread_mutex_destroy(MainWindow::mutexsum);
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    process(threadDelay);
}

void MainWindow::process(int delay)
{
    if(timer->isActive()) timer->stop();
    lockMutex = ui->lockBox->isChecked();
    QString qsthreadNumber = ui->threadNumber->text();
    bool check;
    threadNumber = qsthreadNumber.toInt(&check);
    imageCnt = 0;
    QString qsBrightness= ui->brightness->text();
    float fBrightness = qsBrightness.toFloat(&check);
    QString qsLocalShadow = ui->localShadow->text();
    float fLocalShadow= qsLocalShadow.toFloat(&check);
    QString qsBackLit= ui->backLit->text();
    float fBackLit= qsBackLit.toFloat(&check);
    qDebug()<<"threadNumber"<<threadNumber<<"\n";
    MyThread *thr[threadNumber];
    int imageSize = imageObject.height() * imageObject.bytesPerLine();
         /* create all threads */
    QTime t;
    t.start();
    for (int i=0; i<threadNumber; ++i) {
        qDebug()<<"creating"<<i<<"\n";
        unsigned char * pTmp = (unsigned char *)malloc(imageSize);
        memcpy (pTmp,imageObject.bits() , imageSize );
        imagebuffers[i] =  pTmp;
        thr[i]= new MyThread;
        thr[i]->setParam(i,imageObject.width(),imageObject.height(),imageObject.bytesPerLine()/imageObject.width(),fBrightness,fLocalShadow,fBackLit,lockMutex,imagebuffers[i]);
        thr[i]->start();
        qDebug()<<"started"<<i<<"\n";
        if (delay>0) usleep(delay*1000);
    }

    for (int i=0; i<threadNumber; ++i) {
        thr[i]->wait(10000);
        qDebug()<<"stoped"<<i<<"\n";
    }
    qDebug( "Time elapsed: %d ms", t.elapsed() );
    ui->status->setText("Done");
    timer->start(1000);
}

void MainWindow::on_LoadImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File1"), "/home/bcheng", tr("Files (*.*)"));
    QByteArray ba = fileName.toLocal8Bit();
    const char *c_str2 = ba.data();
    printf("str2: %s", c_str2);
    imagePre = 0;
    imageCnt = 0;
    QImage image;
    image.load(c_str2);
    imageObject= image.convertToFormat(QImage::Format_RGB888, Qt::AutoColor);
    imageObjectOut= image.convertToFormat(QImage::Format_RGB888, Qt::AutoColor);
    ui->label->setPixmap(QPixmap::fromImage(imageObjectOut));
}

void MainWindow::on_NextImage_clicked()
{
    if(timer->isActive()) timer->stop();
    qDebug()<<"in timer1\n";
    imagePre = imageCnt;
    imageCnt++;
    if (imageCnt==threadNumber) imageCnt=0;
    QString qsCurrentImage;
    qsCurrentImage.sprintf("image %d",imageCnt);
    ui->currentImage->setText(qsCurrentImage);
    timer2->start(2);
    qDebug()<<"timer2 started\n";
}

void MainWindow::update()
{
    if(timer->isActive()) timer->stop();
    qDebug()<<"in timer1\n";
    imagePre = imageCnt;
    imageCnt++;
    if (imageCnt==threadNumber) imageCnt=0;
    QString qsCurrentImage;
    qsCurrentImage.sprintf("image %d",imageCnt);
    ui->currentImage->setText(qsCurrentImage);
    timer2->start(100);
    qDebug()<<"timer2 started\n";

}

void MainWindow::slideImage(){
    int imageSize = imageObject.height() * imageObject.bytesPerLine();
    slideStep += 20;
    if(slideStep>=imageObjectOut.height()){
        slideStep = 0;
        memcpy (imageObjectOut.bits() ,imagebuffers[imageCnt], imageSize );
        MainWindow::ui->label->setPixmap(QPixmap::fromImage(imageObjectOut));
        timer2->stop();
        timer->start(1000);
    }else{
       int imageSize1 = slideStep * imageObject.bytesPerLine();
       int imageSize2 = (imageObjectOut.height()-slideStep) * imageObject.bytesPerLine();
       memcpy (imageObjectOut.bits() ,imagebuffers[imageCnt], imageSize1 );
       memcpy (imageObjectOut.bits()+imageSize1 ,imagebuffers[imagePre], imageSize2 );
       MainWindow::ui->label->setPixmap(QPixmap::fromImage(imageObjectOut));
   }

 }



void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    qDebug()<<"position="<<value<<"\n";
    QString qstr;
    qstr.sprintf("Delay %d ms", value);
    ui->delayms->setText(qstr);
    threadDelay = value;
}


void MainWindow::on_pushButton_pressed()
{
     ui->status->setText("Processing ...");
}
