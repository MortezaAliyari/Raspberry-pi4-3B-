#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <LPSFFT.h>
#include <vector>
#include <cmath>
#include <lcd.h>
#include <wiringPi.h>
#include <unistd.h>
#include <QIODevice>
#include <QString>
#include <iostream>
#include <QDebug>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include<QSerialPort>

LPSAN_V57ModelClass test1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//filename: Date and system current time
    //.........
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time (&rawtime);

    timeinfo = localtime(&rawtime);
    strftime(buffer,sizeof(buffer),"%d-%m-%Y_%H-%M-%S",timeinfo);
    string str(buffer);
    string path="/home/pi/Desktop/log/";
    string filename= path+"LPS"+str+".txt";
    myfile.open(filename,std::ios::app);
// Initialize LCD , Serial port and open serial port
    //.........
    SerialPort = new QSerialPort(this);
    SerialLcdInit();
    connect(SerialPort,SIGNAL(readyRead()),this,SLOT(recievePacket()));
// Define timer as timout function//
    //.........
    t = new QTimer(this);
    t->setInterval(0);
    t->setSingleShot(true);
    connect(t, SIGNAL(timeout()), SLOT(TimeOutRecieve()));
    t->start();
    //.........
    test1.initialize();

    if (SerialPort->isOpen()==0){
            SerialPort->close();
    }


}


MainWindow::~MainWindow()
{
    delete ui;
    SerialPort->close();
    lcdClear(lcd);
    myfile.close();

}
void MainWindow::recievePacket()
{
    quint8 data;
    QByteArray packet=SerialPort->QIODevice::read(1);
    if(!packet.isEmpty())
    {
        data=packet[0];
       // qDebug()<<data<<endl;
        if (Headerflag==1){
                Data[i]=data;
                i++;

                if (i>2007)
                {
                    j++;
                   //qDebug()<<j<<endl;
                    i=0;
                    Headerflag=0;
                    showpacket(Data);
                //    LED Blink with each recieved packet
                    digitalWrite(LED,HIGH);
                    delay(50);
                    digitalWrite(LED,LOW);
                    delay(10);

                }
        }
        else if (data==77 && i==0  ){
            Data[i]=data;
            //  qDebug()<<data<<" 1"<<endl;
             i++;
             }
        else if (data==0 && i==1  ){
            Data[i]=data;
           //   qDebug()<<data<<" 2"<<endl;
             i++;
             }
        else if (data==77 && i==2 ){
            Data[i]=data;
          //    qDebug()<<data<<" 3"<<endl;
             i++;
             }
        else if (data==0 && i==3) {
            Data[i]=data;
           //  qDebug()<<data<<" 4"<<endl;
            i++;
            Headerflag=1;
        }
        else {
            i=0;
            Headerflag=0;
        }
}

}

void MainWindow::TimeOutRecieve()
{
    //Stop timer
t->stop();

    if(digitalRead(BTReset)==LOW){

        //sleep(2);
        lcd=lcdInit(2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0);
        lcdClear(lcd);
        Tot=0;
        Err=0;
        qDebug()<<Tot<<' '<<Err<<endl;
        myfile.close();
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];
        timeinfo = localtime(&rawtime);
        strftime(buffer,sizeof(buffer),"%d-%m-%Y_%H-%M-%S",timeinfo);
        string str(buffer);
        string path="/home/pi/Desktop/log/";
        string filename= path+"LPS"+str+".txt";
        myfile.open(filename,std::ios::app);
    }

// If there is no data on bus
   if (SerialPort->waitForReadyRead(10000)==false){
//       timout++;
      t->start();
       i=0;
      // stimout.append(QString::number(timout));
       lcdClear(lcd);
       lcdPosition(lcd,2,0);
       lcdPuts(lcd,"No Signal...");

//       lcdPosition(lcd,14,0);
//       lcdPuts(lcd,stimout);
       //qDebug()<<"No Signal"<<endl;
   }
   else{
          t->start();
   }

}

void MainWindow::showpacket(quint8 Data[]){
quint8 d[2]={0};
quint16 Vn=1000;
qint16 H1,H2,Mode=0,Count=0,lps[1000];
float Fs=1000000,index=0,L=131072;// Initial Sampling Frequency
quint32 lpsfreq=0;
qint32 Vabs[131072],valuesV=0;

//vector<int>::iterator it;

d[0]=Data[0];
d[1]=Data[1];
memcpy(&H1,d,sizeof(qint16));
//qDebug()<<H1<<endl;
d[0]=Data[2];
d[1]=Data[3];
memcpy(&H2,d,sizeof(qint16));
//qDebug()<<H2<<endl;
d[0]=Data[4];
d[1]=Data[5];
memcpy(&Mode,d,sizeof(qint16));
//qDebug()<<Mode<<endl;
d[0]=Data[2*Vn+6];
d[1]=Data[2*Vn+7];
memcpy(&Count,d,sizeof(qint16));
//qDebug()<<Count<<endl;
for(int i=0;i<Vn;i++){
    d[0]=Data[2*i+6];
    d[1]=Data[2*i+7];
    memcpy(&lps[i],d,sizeof(qint16));
    test1.LPSFFT_U.V[i]=lps[i];
}

test1.step(); //Run Matlab fft library

for (quint32 i=0; i<L; i++)
    Vabs[i]=qint32(ceil(sqrt(test1.LPSFFT_Y.Vfftreal[i]*test1.LPSFFT_Y.Vfftreal[i]+test1.LPSFFT_Y.Vfftimag[i]*test1.LPSFFT_Y.Vfftimag[i])));

vector<qint32> dest(begin(Vabs),end(Vabs));
valuesV=*std::max_element(dest.begin(),dest.end());
auto it =std::find(dest.begin(), dest.end(), valuesV);

    // If an Index element was found in LPS signal as frequency index
    if (it != dest.end())
    {
        index = quint16(it - dest.begin()+1);//add one to index deu to arrays start from zero.
        lpsfreq= index*(Fs/L);
//        qDebug()<<index<<" "<<lpsfreq<<endl;
//                     qDebug()<<H1<<endl;
//                     qDebug()<<H2<<endl;
//        if (index==1)
//        {
//             qDebug()<<H1<<endl;
//             qDebug()<<H2<<endl;
////            qDebug()<<"ss"<<endl;
////            for(int jj=0;jj<1000;jj++)
////                qDebug()<<lps[jj]<<endl;

//        }

//            qDebug()<<"ss"<<endl;
//            qDebug()<<"ff"<<endl;
//            for(int jj=0;jj<20000;jj++)
//                qDebug()<<Vabs[jj]<<endl;
//            qDebug()<<"ff"<<endl;
        //qDebug()<<index<<" "<<lpsfreq<<endl;
        if(lpsfreq<=103000 & lpsfreq>=97000){

            float deltafreq=floor(((100000-float(lpsfreq))/33.33)*100.)/100.;

            QString a[10],s,sfreq,err,tot;

            QByteArray ss,stot,SF,sErr;
            s = QString::number(deltafreq);
            sfreq = QString::number(lpsfreq);

            err = QString::number(Err);
            tot = QString::number(Tot);

            Tot++;
            SF.append(sfreq);
            ss.append("$#");
            ss.append(s);
            ss.append("23##\n\r");
            stot.append(tot);
            if (Tot%10==0)
                SerialPort->write(ss);

            lcdClear(lcd);

            lcdPosition(lcd,0,0);
            lcdPuts(lcd,"freq:");
            lcdPosition(lcd,6,0);
            lcdPuts(lcd,SF);

            lcdPosition(lcd,0,1);
            lcdPuts(lcd,"Tot:");
            lcdPosition(lcd,4,1);
            lcdPuts(lcd,stot);

            sErr.append(err);
            lcdPosition(lcd,8,1);
            lcdPuts(lcd,"Err:");
            lcdPosition(lcd,12,1);
            lcdPuts(lcd,sErr);
          //  qDebug()<<deltafreq<<' '<<ss<<' '<<stot<<' '<<endl;
        }
        else{
            Err++;
        }
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer,sizeof(buffer),"%d-%m-%Y_%H-%M-%S",timeinfo);
        string str(buffer);
                myfile << lpsfreq;
                myfile <<"\t";
                myfile << Tot;
                myfile <<"\t";
                myfile << Err;
                myfile <<"\t";
                myfile << str;
                myfile <<"\t\n";
                myfile.seekp(0,std::ios::end);
    }
    else {
       cout << "-1" << endl;
    }
}

void MainWindow::SerialLcdInit()
{
    wiringPiSetup();
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW); // LED
    pinMode(RW,OUTPUT);
    digitalWrite(RW,LOW);//Read/Write is Low -> to ground
    pinMode(BTReset,INPUT); // Bottom : defualt is pull up
   // digitalWrite(25,LOW);
    lcd=lcdInit(2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0);
    //delay(50);
  // lcdDisplay (lcd, true) ;
    lcdClear(lcd);


    SerialPort->setPortName("/dev/ttyS0");
    SerialPort->setBaudRate(115200);
    SerialPort->setDataBits(QSerialPort::Data8);
    SerialPort->setParity(QSerialPort::NoParity);
    SerialPort->setStopBits(QSerialPort::OneStop);
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    SerialPort->setReadBufferSize(1);
    if (SerialPort->open(QIODevice::ReadWrite)) {

//        lcdPosition(lcd,0,1);
//        lcdPuts(lcd,"Connected");
//        delay(3000);
//        lcdClear(lcd);
    qDebug()<<"Connectedd";
    }
    else {
//        lcdPosition(lcd,0,1);
//        lcdPuts(lcd,"Connection Error");
//        sleep(3);
//        lcdClear(lcd);
    qDebug()<<"Not Connectedd";
    }

    if (lcd==-1){

// lcdClear(lcd);
//  lcdPosition(lcd,0,0);
//  lcdPuts(lcd,"LCD Error");
//  delay(5000);
//  lcdClear(lcd);
        qDebug()<<"LCD Error";
    }
    else{
       // lcdClear(lcd);
        lcdPosition(lcd,1,0);
        lcdPuts(lcd,"Please wait ...");
        delay(5000);
        lcdClear(lcd);
        qDebug()<<"please wait.."<<endl;
    }

}









