#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPushButton>
#include <LPSFFT.h>
#include <fstream>
#include <ctime>
#include<QTimer>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    #define RW 3
    #define LCD_RS 2
    #define LCD_E 12
    #define LCD_D4 13
    #define LCD_D5 14
    #define LCD_D6 21
    #define LCD_D7 22
    #define LED 24
    #define BTReset 25


    QByteArray stimout;
    quint16 timout=0;
    int16_t i=0,j=0;
    quint8 Data[2008];
    quint8 Headerflag=0;
    quint16 Err=0,Tot=0;
    ofstream myfile;
    int lcd;
    void showpacket(quint8 Data[]);
    void SerialLcdInit();
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void recievePacket();
   void TimeOutRecieve();

private:
    QSerialPort *SerialPort;
    QTimer *t;
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
