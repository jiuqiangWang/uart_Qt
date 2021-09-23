#include "comdialog.h"
#include "ui_comdialog.h"
#include <QMessageBox>
#include <QDataStream>  //QString 类头文件

ComDialog::ComDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("串口配置"));
    myCom = NULL;

    //查找可用串口
     search_usart();
}

ComDialog::~ComDialog()
{
    delete ui;
}

void ComDialog::search_usart()
{
    ui->PortBox->clear();
    //查找可用串口

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());
            serial.close();
        }
    }
}

void ComDialog::on_Com_Cancel_clicked()
{
    this->close();
}

//com确认连接
void ComDialog::on_ComEnter_clicked()
{
    if(myCom != NULL)
    {
        delete myCom;
    }
    WorkOpen();
    this->close();
}

bool ComDialog::WorkOpen()
{
    //选择串口号
    QString PortName = ui->PortBox->currentText();
    myCom = new QSerialPort(PortName);

    //串口数据连接
    connect(myCom,SIGNAL(readyRead()),this,SLOT(ReadMyCom()));
    //设置波特率
    myCom->setBaudRate(ui->BaudBox->currentText().toInt());

    //设置数据位数
    switch(ui->BitNumBox->currentIndex())
    {
    case 8: myCom->setDataBits(QSerialPort::Data8); break;
    default: break;
    }
    //设置奇偶校验
    switch(ui->ParityBox->currentIndex())
    {
    case 0: myCom->setParity(QSerialPort::NoParity); break;
    default: break;
    }
    //设置停止位
    switch(ui->StopBox->currentIndex())
    {
       case 1: myCom->setStopBits(QSerialPort::OneStop); break;
       case 2: myCom->setStopBits(QSerialPort::TwoStop); break;
       default: break;
    }

    //以可读写方式打开串口
    bool flag = myCom->open(QIODevice::ReadWrite);

    if(false == flag)
    {
        QMessageBox::warning(this,tr("Warning"),tr("串口不存在或已被占用!"),QMessageBox::Yes);
        return false;
    }

    ui->ComStartlabel->setText(tr("已连接"));
    main_ui->label_connect_state->setText("连接成功");
    m_isConnect=true;
    main_ui->SendPushButton->setEnabled(true);//enable

}

bool ComDialog::WorkClose()
{
    if(!m_isConnect)
    {
        this->close();
        return 0;
    }
    myCom->close();
    ui->ComStartlabel->setText(tr("未连接"));
    main_ui->label_connect_state->setText("未建立连接");
    main_ui->SendPushButton->setEnabled(false);
    m_isConnect = false;
    this->close();
    return true;
}



void ComDialog::ReadMyCom()
{

    //读取串口数据
    QByteArray byte_data = myCom->readAll();

    //将读取到的数据进行转化
    QString strDisplay;

    if(m_isHexDisplay)//十六进制显示
    {
        QString str = byte_data.toHex().data();
        str = str.toUpper();//转换为大写
        for(int i = 0;i<str.length();i+=2)
        {
            QString st = str.mid(i,2);//返回一个字符串 从指定位置返回n个字符
            strDisplay += st;
            strDisplay += " ";//每个字节中间的空格
        }
    }
    else//字符显示
    {
          strDisplay = QString(byte_data);
    }

    if(true == DisplayFlag)
    {
        main_ui->ReceiveDatatextBrowser->insertPlainText(strDisplay);
        //接收太多数据后，会变得很慢。发送和接收的速度变得不正常。
        main_ui->ReceiveDatatextBrowser->moveCursor(QTextCursor::End);
    }


}

  void ComDialog::WorkSendMessage()
  {
      if(!(myCom->isOpen()))return;

      QString sdata = main_ui->textEdit->toPlainText();

      if(sdata.isEmpty())return;

      QByteArray sendBuf = "";

      if(m_isHexSend)
      {
          sendBuf = QByteArray::fromHex(sdata.toLatin1().data());
      }
      else
      {
          sendBuf =  sdata.toUtf8(); /*怎么以字符的形式发送出去*/
      }
      myCom->write(sendBuf);
  }










