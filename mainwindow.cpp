#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QThread>
#include <QTimer>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//im_io->data_array = (byte **)malloc(im_io->height * sizeof(void *));
//imagebuffers = (unsigned char**)malloc(NUM_THREADS * sizeof(void *));
    imageCnt = 0;
    ui->setupUi(this);
        ui->delayms->setText("Delay 0 ms");
QImage image;
         image.load("/home/bcheng/tt4.jpg");
//          QImage image(300, 300, QImage::Format_RGB32);
//         for (int i = 0; i<100; i++){
//             for (int j = 0; j < 100; j++){
//                 QRgb value;

//                 value = qRgb(189, 149, 39); // 0xffbd9527
//                imageObject.setPixel(i,j,value);

//             }
//         }
imageObject= image.convertToFormat(QImage::Format_RGB888, Qt::AutoColor);
int w1= image.width();
int w2 = imageObject.width();
         ui->label->setPixmap(QPixmap::fromImage(imageObject));
}

MainWindow::~MainWindow()
{
    if(timer->isActive()) timer->stop();
//    for (int i=0; i<NUM_THREADS; ++i) {
//        if(imagebuffers[i]!=NULL){
//            free(imagebuffers[i]);
//        }
//    }
//    if(imagebuffers!=NULL){
//        free(imagebuffers);
//    }
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

process(threadDelay);
}


void MainWindow::start()
{

}

void MainWindow::process(int delay)
{
threadDelay=0;
    if(timer->isActive()) timer->stop();
    QString qsthreadNumber = ui->threadNumber->text();
     bool check;
    threadNumber = qsthreadNumber.toInt(&check);
qDebug()<<"threadNumber"<<threadNumber<<"\n";
    MyThread *thr[threadNumber];
   //  unsigned char *imagebuffers[NUM_THREADS];
  int imageSize = imageObject.height() * imageObject.bytesPerLine();
         /* create all threads */
         for (int i=0; i<threadNumber; ++i) {
          //  thread_args[i] = i;
            qDebug()<<"creating"<<i<<"\n";
          //  if(*(imagebuffers+i)==NULL){
                unsigned char * pTmp = (unsigned char *)malloc(imageSize);
                 memcpy (pTmp,imageObject.bits() , imageSize );
              //   im_io->data_array[row] = (byte *)(imagebuffer + rowbytes * row);
                imagebuffers[i] =  pTmp;
         //   }

            thr[i]= new MyThread;
            thr[i]->setParam(i,imageObject.width(),imageObject.height(),imageObject.bytesPerLine()/imageObject.width(),imagebuffers[i]);
                thr[i]->start();
                qDebug()<<"started"<<i<<"\n";
                if (delay>0) usleep(delay*1000);
         }

         for (int i=0; i<threadNumber; ++i) {
           thr[i]->wait(10000);
           qDebug()<<"stoped"<<i<<"\n";
         }
         //int imageSize = imageObject.height() * imageObject.bytesPerLine();

//         memcpy (imageObject.bits() ,imagebuffers[0], imageSize );
//   MainWindow::ui->label->setPixmap(QPixmap::fromImage(imageObject));
 ui->status->setText("Done");
    timer->start(1000);

 }



void MainWindow::on_LoadImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Open File1"), "/home/bcheng", tr("Files (*.*)"));
    QByteArray ba = fileName.toLocal8Bit();
    const char *c_str2 = ba.data();
    printf("str2: %s", c_str2);

    imageObject.load(c_str2);

    ui->label->setPixmap(QPixmap::fromImage(imageObject));

}

void MainWindow::on_NextImage_clicked()
{
    if(timer->isActive()) timer->stop();
    update();
}

void MainWindow::update()
{
    int imageSize = imageObject.height() * imageObject.bytesPerLine();
   imageCnt++;
   if (imageCnt==threadNumber) imageCnt=0;
//    memcpy (imageObject.bits() ,*(imagebuffers+imageCnt) , imageSize );
//  ui->label->setPixmap(QPixmap::fromImage(imageObject));

    memcpy (imageObject.bits() ,imagebuffers[imageCnt], imageSize );
MainWindow::ui->label->setPixmap(QPixmap::fromImage(imageObject));

 }

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{

}

void MainWindow::on_horizontalSlider_sliderReleased()
{

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    qDebug()<<"position="<<value<<"\n";
    QString qstr;
    qstr.sprintf("Delay %d ms", value);
    ui->delayms->setText(qstr);
    threadDelay = value;
 //   process(value);
}
