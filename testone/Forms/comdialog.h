#ifndef COMDIALOG_H
#define COMDIALOG_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>           //提供访问串口的功能 wjq 2018.7.7 1-17-2019
#include <QtSerialPort/QSerialPortInfo>       //提供系统中存在的串口的信息 wjq 2018.7.7 Jannuary 17th 2019
#include "ui_mainwindow.h"

namespace Ui {
class ComDialog;
}

class ComDialog : public QDialog //派生类Combase and QDialog  今天到这里 明天上班学习一下 菜鸟教程C++的继承里面的基类&派生类
{
    Q_OBJECT

public:
    explicit ComDialog(QWidget *parent = 0);
    ~ComDialog();
    QSerialPort *myCom;//这个是公共的

    void setMainUiPoint(Ui::MainWindow *ui){main_ui = ui;} //指针在子界面调用主界面的部件
    void WorkSetDisplayFlag(bool flag){DisplayFlag = flag;}
    bool WorkOpen();                        //打开串口
    bool WorkClose();                       //关闭串口
    void WorkSendMessage();                 //发送数据

    bool m_isConnect;  //是否已经连上

    void ReadMyCom();  //读取串口数据
    Ui::ComDialog *ui;

    void search_usart();                     //查找串口

    bool m_isHexDisplay; //是否16进制显示
    bool m_isHexSend; //是否16进制发送

private slots:
    void on_Com_Cancel_clicked();//com config cancel
    void on_ComEnter_clicked();//com config ok

private:
  //等一下来做实验   在子界面中放一个button 和 label ，然后在主界面中放一个button 和 label ，分别用button 显示 字符。
    Ui::MainWindow  *main_ui;//这样就可以在子界面中   设置主界面的label了。
    bool DisplayFlag;  //停止显示

};

#endif // COMDIALOG_H
