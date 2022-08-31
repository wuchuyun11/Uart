#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSerialPort/QSerialPortInfo>
#include <QMainWindow>
#include <QtSerialPort>
#include <QString>
#include <QIODevice>
#include <QList>
#include <QSerialPortInfo>
#include <QObject>
#include <QAction>
#include <QDebug>
#include <QByteArray>
#include <QDateTime>
#include <QFileDialog>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QString inipath;
    explicit MainWindow(QWidget *parent = 0);
    void InitPort();
    ~MainWindow();
    void ini_repalce(QString &receshow);
    bool getserialPortconfig(void);
    bool openTextByIODevice(const QString &aFileName);
    void StringToHex(QString str, QByteArray &senddata);
private slots:

void on_ButtonOpen_clicked();

void on_pushButton_3_clicked();

void on_mserialport_readyRead();

void on_pushButton_clicked();

void on_pushButton_2_clicked();

void on_pushButton_5_clicked();


void on_pushButton_4_clicked();

void on_pushButton_6_clicked();

void on_Timetosend();

char convertHexChart(char ch);  //16进制转换

void IniParamInit(void);

bool on_pushButton_7_clicked();


void on_pushButton_8_clicked();

private:
    QString mportname;
    QString mbaudrate;
    QString mstopbits;
    QString mparity;
    QString mdatabits;
    QSerialPort mserialport;
    QTimer *time;
    Ui::MainWindow *ui;
    bool misOpen;
};






#endif // MAINWINDOW_H
