# Raspberry pi4B




This project is included four main parts as following:
1. Serial communication
2. 131K sample FFT
3. log file 
4. 2*16 LCD
Summary:
    In this project, about 1000 samples of data(int16) received by serial port of raspberry pi4b and used the FFT algorithm with 131072 points to calculate the main frequency of receiving signal. If the calculated frequency is in the frequency range it will send to another device by serial port. All information logged in a text file. The file and its information labelled with the current date and time for each runtime. The Whole project was written by QT5 in the Rasbian Linux distro.
1. Serial communication
To use this kind of communication you will need to :
1.1 Enable serial flag and disable serial console as we did in raspberry pi3,2
1.2 Add QT+=serailport to .pro file in QT5 
1.3 Sudo apt-get install libqt5serialport5
1.4 Sudo apt-get install libqt5serialport5-dev
1.5 Use below libs:
	#include <QtSerialPort/QSerialPort>
	#include <QtSerialPort/QSerialPortInfo>
Now Serial port is ready to be used.
2. FFT with 131072 points sample:
By using Matlab/Simulink code generation (C++) a .h and .c files will be produced in order to be used and added to the QT project.
FFT function finds the number of input samples itself each time.
3. log file:
The calculated frequency, number of received serial packets which is in the frequency range count. Also, a counter allocated to FFT error increases if the calculated frequency is out of range. So three parameters will be logged with the current date and time in a text file. The date and time will set for the first time and the board should be on all the time!
4. LCD:
The calculated frequency displayed on 2*16 LCD. Qt5 software has an LCD library and no need for installation. Also, if there is no data after 10 seconds on the serial port, the LCD will show a "No signal" message.
The project source code attached. The code not optimized to have better performance!
***********************************************Ask if you see any problem!**************************************

Mainwidow.c:
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
        string filename= path+"Aliyari"+str+".txt";
        myfile.open(filename,std::ios::app);
    }

// If there is no data on bus, The LCD wil show timeout
   if (SerialPort->waitForReadyRead(10000)==false){
      t->start();
       i=0;
       lcdClear(lcd);
       lcdPosition(lcd,2,0);
       lcdPuts(lcd,"No Signal...");
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

d[0]=Data[0];
d[1]=Data[1];
memcpy(&H1,d,sizeof(qint16));

d[0]=Data[2];
d[1]=Data[3];
memcpy(&H2,d,sizeof(qint16));

d[0]=Data[4];
d[1]=Data[5];
memcpy(&Mode,d,sizeof(qint16));

d[0]=Data[2*Vn+6];
d[1]=Data[2*Vn+7];
memcpy(&Count,d,sizeof(qint16));

for(int i=0;i<Vn;i++){
    d[0]=Data[2*i+6];
    d[1]=Data[2*i+7];
    memcpy(&lps[i],d,sizeof(qint16));
    test1.LPSFFT_U.V[i]=lps[i];
}

test1.step(); //Run Matlab fft functiuon 

for (quint32 i=0; i<L; i++)
    Vabs[i]=qint32(ceil(sqrt(test1.LPSFFT_Y.Vfftreal[i]*test1.LPSFFT_Y.Vfftreal[i]+test1.LPSFFT_Y.Vfftimag[i]*test1.LPSFFT_Y.Vfftimag[i])));

vector<qint32> dest(begin(Vabs),end(Vabs));
valuesV=*std::max_element(dest.begin(),dest.end());
auto it =std::find(dest.begin(), dest.end(), valuesV);

    // If an Index element was found from signal as frequency index
    if (it != dest.end())
    {
        index = quint16(it - dest.begin()+1);//add one to index deu to arrays start from zero.
        lpsfreq= index*(Fs/L);

        if(lpsfreq<=103000 & lpsfreq>=97000){ // Prefer Frequency Range

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
            if (Tot%10==0)// Calculate Frequency will send each  10 times
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
   
}

void MainWindow::SerialLcdInit()
{
    wiringPiSetup();
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW); // LED
    pinMode(RW,OUTPUT);
    digitalWrite(RW,LOW);//Read/Write is Low -> to ground
    pinMode(BTReset,INPUT); // Bottom : defualt is pull up
    lcd=lcdInit(2,16,4,LCD_RS,LCD_E,LCD_D4,LCD_D5,LCD_D6,LCD_D7,0,0,0,0);
     lcdClear(lcd);


    SerialPort->setPortName("/dev/ttyS0");
    SerialPort->setBaudRate(115200);
    SerialPort->setDataBits(QSerialPort::Data8);
    SerialPort->setParity(QSerialPort::NoParity);
    SerialPort->setStopBits(QSerialPort::OneStop);
    SerialPort->setFlowControl(QSerialPort::NoFlowControl);
    SerialPort->setReadBufferSize(1);
    if (SerialPort->open(QIODevice::ReadWrite)) {

    qDebug()<<"Connectedd";
    }
    else {

    qDebug()<<"Not Connectedd";
    }

    if (lcd==-1){

        qDebug()<<"LCD Error";
    }
    else{
        lcdPosition(lcd,1,0);
        lcdPuts(lcd,"Please wait ...");
        delay(5000);
        lcdClear(lcd);
        qDebug()<<"please wait.."<<endl;
    }

}









