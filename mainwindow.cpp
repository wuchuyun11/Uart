#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time= new QTimer(this);
    InitPort();
    IniParamInit();
    misOpen=false;
    //获取当前有效串口号
    QList<QSerialPortInfo> serialPortInfo= QSerialPortInfo::availablePorts();
    int count=serialPortInfo.count();
    for(int i=0;i<count;i++)
    {
        ui->CboxPortname->addItem(serialPortInfo.at(i).portName());
    }
    connect(ui->checkBox_7,&QCheckBox::stateChanged,this,&MainWindow::on_Timetosend);
    connect(&mserialport,SIGNAL(readyRead()),this,SLOT(on_mserialport_readyRead()));
    connect(time,&QTimer::timeout,this,&MainWindow::on_pushButton_3_clicked);
//    connect(ui->pushButton_8,&QPushButton::clicked,this,&MainWindow::on_pushButton_8_clicked);
}
void MainWindow::InitPort()
{

    QStringList baudList;   //波特率
    QStringList parityList; //校验位
    QStringList dataBitsList;   //数据位
    QStringList stopBitsList;   //停止位
    // 波特率    //波特率默认选择下拉第三项：9600
    baudList<<"1200"<<"2400"<<"4800"<<"9600"
           <<"19200"<<"38400";
    ui->CboxBaudrate->addItems(baudList);

    // 校验      //校验默认选择无
    parityList<<"无"<<"奇"<<"偶";
    ui->CboxParity->addItems(parityList);

    // 数据位      //数据位默认选择8位
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->CboxDatabits->addItems(dataBitsList);

    // 停止位      //停止位默认选择1位
    stopBitsList<<"1"<<"1.5"<<"2";
    ui->CboxStopbits->addItems(stopBitsList);


//    ui->ButtonOpen->setEnabled(true);
//    ui->radioButton_2->setEnabled(true);
//    ui->checkBox_3->setChecked(true);
//    ui->checkBox_4->setChecked(false);
//    ui->radioButton->setChecked(true);
//    ui->radioButton_3->setChecked(true);
}
MainWindow::~MainWindow()
{
    delete ui;
    delete time;
}
bool MainWindow::getserialPortconfig()
{
    //获取串口配置
    mportname= ui->CboxPortname->currentText();
    mbaudrate=ui->CboxBaudrate->currentText();
    mparity=ui->CboxParity->currentIndex();
    mstopbits=ui->CboxStopbits->currentIndex();
    mdatabits=ui->CboxDatabits->currentText();
    //设置串口名
    mserialport.setPortName(mportname);
    //波特率
    if(mbaudrate==1200)
    {
        mserialport.setBaudRate(QSerialPort::Baud1200);

    }
    if(mbaudrate==4800)
    {
        mserialport.setBaudRate(QSerialPort::Baud4800);

    }
    if(mbaudrate==9600)
    {
        mserialport.setBaudRate(QSerialPort::Baud9600);

    }
    if(mbaudrate==19200)
    {
        mserialport.setBaudRate(QSerialPort::Baud19200);

    }
    //波特率
    if(mbaudrate==38400)
    {
        mserialport.setBaudRate(QSerialPort::Baud38400);
    }
    //校验位
    switch (ui->CboxParity->currentIndex())
    {
        case 0:
            mserialport.setParity(QSerialPort::NoParity);
            break;
        case 1:
            mserialport.setParity(QSerialPort::OddParity);
            break;
        case 2:
            mserialport.setParity(QSerialPort::EvenParity);
            break;
        default:
            break;
     }
    //停止位
    switch (ui->CboxStopbits->currentIndex())
    {
    case 0:
        mserialport.setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        mserialport.setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        mserialport.setStopBits(QSerialPort::TwoStop);
        break;
    default:
        break;
    }
    //数据位
    switch (ui->CboxDatabits->currentIndex()) {
    case 5:
        mserialport.setDataBits(QSerialPort::Data5);
        break;
    case 6:
        mserialport.setDataBits(QSerialPort::Data6);
        break;
    case 7:
        mserialport.setDataBits(QSerialPort::Data7);
        break;
    case 8:
        mserialport.setDataBits(QSerialPort::Data8);
        break;
    default:
        break;
    }
    return mserialport.open(QSerialPort::ReadWrite);
    
}
//点击打开按钮
void MainWindow::on_ButtonOpen_clicked()
{
    if(true==misOpen)
    {
        //串口已打开，执行关闭操作
        mserialport.close();
        misOpen=false;
        ui->ButtonOpen->setText("打开串口");
        ui->CboxPortname->setEnabled(true);
        ui->CboxBaudrate->setEnabled(true);
        ui->CboxParity->setEnabled(true);
        ui->CboxStopbits->setEnabled(true);
        ui->CboxDatabits->setEnabled(true);

    }
    else if(true==getserialPortconfig())
    {
        qDebug()<<"打开成功"<<mportname;
        misOpen=true;
        ui->ButtonOpen->setText("关闭串口");
        ui->CboxPortname->setEnabled(false);
        ui->CboxBaudrate->setEnabled(false);
        ui->CboxParity->setEnabled(false);
        ui->CboxStopbits->setEnabled(false);
        ui->CboxDatabits->setEnabled(false);
    }



}
//发送
void MainWindow::on_pushButton_3_clicked()
{
    if(ui->ButtonOpen->text() == "打开串口")
    {
        QMessageBox::warning(NULL, "警告", "未打开可用串口，无法发送数据！\r\n\r\n");
        ui->checkBox_7->setChecked(false);
        return;
    }

    QString str=ui->lineEdit_4->text();
        if(misOpen)
        {
            QByteArray senddata;
            if(true==ui->radioButton_4->isChecked())
            {
                StringToHex(str,senddata);
                mserialport.write(senddata);
                if(mserialport.write(senddata)<0)
                {
                    QMessageBox::critical(this, tr("Error"), mserialport.errorString());
                }
            }
            else
            {
                if(mserialport.write(ui->lineEdit_4->text().toLocal8Bit())<0)
                {
                    QMessageBox::critical(this, tr("Error"), mserialport.errorString());
                }

            }

        }
}
//接收信号
void MainWindow::on_mserialport_readyRead()
{
    QString timeStr;
    QString receshow="";
    QByteArray recedata=mserialport.readAll();
    QString str=QString(recedata);
    if(true==ui->checkBox_3->isChecked())
    {
        QDateTime timeCurrent = QDateTime::currentDateTime();
        timeStr = timeCurrent.toString("[yy-MM-dd hh:mm:ss]");
        ui->textEdit->append(timeStr);
    }

    //ASCII码接收
    if(true==ui->radioButton->isChecked())
    {
         receshow = QString::fromLocal8Bit(recedata);
//        receshow.append(myStrTemp);
//        ui->textEdit->append(receshow);
        qDebug()<<receshow.size();
        if(true==ui->checkBox_2->isChecked())
        {
            QString filter=ui->lineEdit_5->text();
            int count=0;
            for(;count!=-1;)
            {
                count=receshow.indexOf(filter);
                receshow.replace(count,filter.size(),"");

            }
        }
        ini_repalce(receshow);
        ui->textEdit->append(receshow);
     }
    //HEX接收
    else if(true==ui->radioButton_2->isChecked())
    {
        QDataStream out(&recedata,QIODevice::ReadOnly);    //将字节数组读入
        ui->textEdit->insertPlainText("\n");
        while(!out.atEnd())
        {
               qint8 outChar = 0;
               out>>outChar;   //每字节填充一次，直到结束
               //十六进制的转换
               QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
               if(1){
                   if(!ui->checkBox_2->isChecked())
                   {
                   ui->textEdit->insertPlainText(str.toUpper());//大写
                   ui->textEdit->insertPlainText(" ");//每发送两个字符后添加一个空格
//                   ui->textEdit->moveCursor(QTextCursor::End);
//                   qDebug()<<"1";
                   }
//                    qDebug()<<str;
                   receshow+=str.toUpper();
                   receshow+=" ";


               }
                                  qDebug()<<receshow.size();
        }
        if(true==ui->checkBox_2->isChecked())
        {
            QString filter=ui->lineEdit_5->text();

            int count=0;
            for(;count!=-1;)
            {
                count=receshow.indexOf(filter);
                receshow.replace(count,filter.size(),"");
            }
            ini_repalce(receshow);
            ui->textEdit->append(receshow);
        }

    }

    if(true==ui->checkBox->isChecked())
    {
         ui->textEdit->insertPlainText("\r\n");
    }
}
//刷新串口
void MainWindow::on_pushButton_clicked()
{
    QList<QSerialPortInfo> nowserialport= QSerialPortInfo::availablePorts();
    int value=nowserialport.count();
    ui->CboxPortname->clear();
    for(int i=0;i<value;i++)
    {

        ui->CboxPortname->addItem(nowserialport.at(i).portName());
    }
}
//清空接受区
void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
}
//清空发送区
void MainWindow::on_pushButton_5_clicked()
{
    ui->lineEdit_4->clear();
}
void MainWindow::on_pushButton_4_clicked()
{
    QString curPath=QDir::currentPath();    //获取系统当前目录
    QString dlgTitle="打开文件";            //对话框标题
    QString filter="文本文件(*.txt);;程序文件(*.h *.cpp);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;
    ui->lineEdit->setText(aFileName);
    openTextByIODevice(aFileName);
}
bool MainWindow::openTextByIODevice(const QString &aFileName)
{
    QFile aFile(aFileName);
    if (!aFile.exists())                //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    ui->lineEdit_4->setText(QString(aFile.readAll()));
    aFile.close();
    return  true;
}

void MainWindow::on_pushButton_6_clicked()
{
    QString curPath=QDir::currentPath();    //获取系统当前目录
    QString dlgTitle="打开文件";            //对话框标题
    QString filter="文本文件(*.txt);;程序文件(*.h *.cpp);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;
    ui->lineEdit_2->setPlaceholderText(aFileName);
    QFile afile(aFileName);
    QString str=ui->textEdit->toPlainText();
    if(afile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QByteArray strbyte=str.toUtf8();
        afile.write(strbyte,strbyte.length());
        afile.close();
    }


}
//定时发送
void MainWindow::on_Timetosend()
{
    if(true==ui->checkBox_7->isChecked())
    {
        if(time->isActive())
        {
            return;
        }
        else{
            int ms=ui->lineEdit_3->text().toInt();
            time->start(ms);
        }
    }
    else{
        if(time->isActive())
        {
            time->stop();
        }
        else{
            return;
        }
    }
}
//转HEX
void MainWindow::StringToHex(QString str, QByteArray &senddata)

{
    int hexdata,lowhexdata;
           int hexdatalen = 0;
           int len = str.length();
           senddata.resize(len/2);
           char lstr,hstr;
           for(int i=0; i<len; )
           {
               //char lstr,
               hstr=str[i].toLatin1();
               if(hstr == ' ')
               {
                   i++;
                   continue;
               }
               i++;
               if(i >= len)
                   break;
               lstr = str[i].toLatin1();
               hexdata = convertHexChart(hstr);
               lowhexdata = convertHexChart(lstr);
               if((hexdata == 16) || (lowhexdata == 16))
                   break;
               else
                   hexdata = hexdata*16+lowhexdata;
               i++;
               senddata[hexdatalen] = (char)hexdata;
               hexdatalen++;
           }
           senddata.resize(hexdatalen);

}
char MainWindow::convertHexChart(char ch)
{
    if((ch >= '0') && (ch <= '9'))
                return ch-0x30;  // 0x30 对应 ‘0’
            else if((ch >= 'A') && (ch <= 'F'))
                return ch-'A'+10;
            else if((ch >= 'a') && (ch <= 'f'))
                return ch-'a'+10;
    //        else return (-1);
    else return ch-ch;//不在0-f范围内的会发送成0

}

void MainWindow::IniParamInit()
{
    QSettings *configIni = new QSettings("uart.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    if(configIni == NULL)
        return;
    //波特率
    QString  baudRate = configIni->value("uartParam/BaudRate").toString();
    ui->CboxBaudrate->setCurrentText(baudRate);
    //数据位
    QString  dataBit = configIni->value("uartParam/DataBit").toString();
    ui->CboxDatabits->setCurrentText(dataBit);
    //奇偶校验位
    QString  parity = configIni->value("uartParam/Parity").toString();
    qDebug()<<"parity:"<<parity;
    ui->CboxParity->setCurrentText(parity);
    //停止位
    QString  stopBit = configIni->value("uartParam/StopBit").toString();
    ui->CboxStopbits->setCurrentText(stopBit);
    //时间戳
    bool  hasTimeStamp = configIni->value("uartParam/timestamp").toBool();
    ui->checkBox_3->setChecked(hasTimeStamp);
    //回车换行
    bool  hasAT = configIni->value("uartParam/AT").toBool();
    ui->checkBox->setChecked(hasAT);
    //HEX发送
    bool  hexSend = configIni->value("uartParam/HEXS").toBool();
    ui->radioButton_4->setChecked(hexSend);
    //HEX显示
    bool  hexRec = configIni->value("uartParam/HEXR").toBool();
    ui->radioButton_2->setChecked(hexRec);
    delete  configIni;
}


bool MainWindow::on_pushButton_7_clicked()
{
    QSettings *configIni = new QSettings("uart.ini", QSettings::IniFormat);
    configIni->setIniCodec("UTF-8");
    if(configIni == NULL)
        return false;
    //波特率
    configIni->setValue("uartParam/BaudRate",ui->CboxBaudrate->currentText());
    //数据位
    configIni->setValue("uartParam/DataBit",ui->CboxDatabits->currentText());
    //奇偶校验位
    configIni->setValue("uartParam/Parity",ui->CboxParity->currentText());
    //停止位
    configIni->setValue("uartParam/StopBit",ui->CboxStopbits->currentText());
    //时间戳
    configIni->setValue("uartParam/timestamp",ui->checkBox_3->isChecked());
    //AT
    configIni->setValue("uartParam/AT",ui->checkBox->isChecked());

    //HEX发送
    configIni->setValue("uartParam/HEXS",ui->radioButton_2->isChecked());
    //HEX显示
    configIni->setValue("uartParam/HEXR",ui->radioButton_4->isChecked());
    delete  configIni;
    return true;

}


void MainWindow::ini_repalce(QString &receshow)
{
        QSettings *configIni = new QSettings(inipath, QSettings::IniFormat);
        configIni->setIniCodec("UTF-8");
        foreach (QString key, configIni->allKeys())
        {
            int count=0;
            for(;count!=-1;)
            {
//                qDebug() << key << configIni->value(key).toString();
                count=receshow.indexOf(key);
                receshow.replace(count,key.size(),configIni->value(key).toString());

            }
        }
}
void MainWindow::on_pushButton_8_clicked()
{
    QString curPath=QDir::currentPath();    //获取系统当前目录
    QString dlgTitle="打开文件";            //对话框标题
    QString filter="配置文件(*.ini);;程序文件(*.h *.cpp);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    inipath=aFileName;
}
