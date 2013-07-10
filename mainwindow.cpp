#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

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
    QImage imageObject;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    start();

  //  ui->label->setPixmap(imageObject);
}

void MainWindow::start()
{
    int imageSize = imageObject.height() * imageObject.bytesPerLine();
    unsigned char* imagebuffer = (unsigned char *)malloc(imageSize);

memcpy ( imagebuffer, imageObject.bits(), imageSize );
    process(imagebuffer);
}

void MainWindow::process(unsigned char *pImage)
{
    HIsEnhance ISObject;
      char guid[]  = "3c3c8316-50d0-948d-ab14-6717e0649e1a";

    ISObject = IsEnhanceCreateFromGUID(guid);

    char str_buffer[256];
     IsEnhancePars newPars;
     IsEnhanceProcessPars newProcessPars;
      int width =imageObject.width();
      int height =imageObject.height();
      int components =imageObject.bytesPerLine()/width;
      IsEnhanceImage IsImage;
        IsImage.height = height;
        IsImage.width = width;
        IsImage.bytesperline = imageObject.bytesPerLine();
        IsImage.imagedata = pImage;
        IsImage.colorOrder = IS_RGB;
        IsImage.memOrg = IS_PixelInterleaved;
        //Setting the default metadata
        IsImage.metadata.flash = 16;
        IsImage.metadata.lightsource = 0;
        IsImage.metadata.sharpness = 0;
        IsImage.metadata.contrast = 0;
        IsImage.metadata.saturation = 0;
        IsImage.metadata.isospeedratings = 200;
        IsImage.metadata.shutterspeed = 5.0;
        IsImage.metadata.exposuretime = 0.03333;
        IsImage.metadata.aperture = 3.5;
        IsImage.metadata.fnumber = 3.0;
        IsImage.metadata.brightness = 1.0;

        IsImage.metadata.is_enhanced = 0;  // 0 = no, >0 = yes. Can be used to avoid repeated
        IsImage.metadata.filesize = 0;  // image file size on disk, 0 if not available
        IsImage.metadata.extResizeFac = 1.0; // factor indicating if image has been resized previously, used e.g.
        // to adapt global sharpening. Set this value to <= 0 or 1.0 if no
        // information is available or the image has not been resized
        // (usual case). A previous up- or down-sampling respectively
        // corresponds to to a factor larger or smaller than 1.0.

        // input ICC profile (embedded profile of input image)
        IsImage.metadata.pIccInProfileData = NULL;  // should contain embedded ICC
        IsImage.metadata.iccInProfileDataSize = 0; // = 0  no emb. ICC profile associated with this image
        // > 0  specifies ICC profile size located at pIccInProfileData
        // ICC profile to be used for output image
        IsImage.metadata.pIccOutProfileData = NULL;
        IsImage.metadata.iccOutProfileDataSize = 0;
        IsImage.metadata.pAlphaChannelData = 0;  // set to 0 if no alpha channel is avail.
        IsImage.metadata.alphaChannelDataSize = 0; // set to width*height, if alpha ch. avail.

          char str_buffer_dir[30];
          int cnt=2;
          sprintf(str_buffer_dir,"/home/bcheng/d/dir%d/Pars.bin",cnt);

      ifstream ifs1(str_buffer_dir, ios::binary);
      ifs1.read((char *)&newPars, sizeof(newPars));
      ifs1.close();


          sprintf(str_buffer_dir,"/home/bcheng/d/dir%d/PPar.bin",cnt);
          fprintf(stderr,str_buffer_dir);
  //        fprintf(stderr,"\n pid: %d; threadID=%d; ISObject= %d; bcheng2\n", getpid() , pthread_self(),ISObject);

    //          pthread_mutex_lock (&mutexsum);
          newPars.gpars.brstrength=1;
      IsEnhanceSetPars(ISObject, &newPars);
      ifstream ifs3(str_buffer_dir, ios::binary);
      ifs3.read((char *)&newProcessPars, sizeof(newProcessPars));
      ifs3.close();



             //              fprintf(stderr,"\nb pid: %d; threadID=%d; ISObject=%d;\n", getpid() , pthread_self(), ISObject);
       int retValue = IsEnhanceProcessAndResizeImage(ISObject, &IsImage, &IsImage, &newProcessPars);
       fprintf(stderr,"retValue=%d\n",retValue);
       if (retValue < 0)
         {
           char errMsg[IS_MAXPATH];
           IsEnhanceGetLastError(ISObject, errMsg);
           fprintf(stderr, "%s\n", errMsg);
         }
       // fprintf(stderr,"a pid: %d; threadID=%d; ISObject=%d; retValue=%d\n", getpid() , pthread_self(),ISObject, retValue);
       //  pthread_mutex_unlock (&mutexsum);
       int imageSize = imageObject.height() * imageObject.bytesPerLine();

   memcpy (imageObject.bits() ,pImage , imageSize );
 ui->label->setPixmap(QPixmap::fromImage(imageObject));

              if(ISObject) IsEnhanceDestroy (ISObject);

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
