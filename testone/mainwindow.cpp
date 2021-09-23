#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>//保存文件要加这个头文件
#include "QTimer"
#include "QObject"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*设置窗口标题*/
    setWindowTitle(tr("调试助手V1.0"));
    //窗口建立资源
    P_Com = NULL;
    P_Com = new ComDialog();
    P_Com->setMainUiPoint(ui);//必须要有这个 才能从子界面的button按下，在主界面的label显示。

    P_About = NULL;
    P_About = new AboutDialog();//关于对话框，没有这句、下面的关联到槽就实现不了。
    /*菜单栏*/
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(MainWindow_Quit()));//关联到信号槽
    connect(ui->actionAbout,SIGNAL(triggered(bool)),this,SLOT(MainWindow_About()));
    connect(ui->actionConnect,SIGNAL(triggered(bool)),this,SLOT(MainWindow_Communication()));
    connect(ui->actionDisconncet,SIGNAL(triggered(bool)),this,SLOT(MainWindow_DisCommunication()));
    connect(ui->actionSave,SIGNAL(triggered(bool)),this,SLOT(on_SavePushButton_clicked()));


    DisplayFlag = true;
    P_Com->WorkSetDisplayFlag(DisplayFlag);//接收按钮初始化
    ui->checkBox->setChecked(false);//默认字符显示模式
    ui->SendPushButton->setEnabled(false);
    ui->AutoSend->setEnabled(true);//定时发送 false

    //串口自动发送定时器
    myTime = new QTimer();
    //myTime->setInterval(2000);
    connect(myTime,SIGNAL(timeout()),this,SLOT(SlectAutoSend()));

    QTimer *timer = new QTimer(this);    //创建一个新的定时器
    //关联定时器的溢出信号到槽上
    connect(timer,&QTimer::timeout,this,&MainWindow::timerUpdata);
    timer->start(1000);  //设置溢出时间为1S，并启动定时器 这个是timerUpdata 刷新时间
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//获取当前时间

    id1 = startTimer(1);//这个是timerEvent 用来和串口建立连接  通讯使用。
    ui->label_connect_state->setText("未建立连接");
}

void MainWindow::timerUpdata()//定时器溢出事件
{
    QTime time = QTime::currentTime();  //获取当前时间
    QString text = time.toString(("hh:mm:ss"));   //转换为字符串

    ui->label_timer->setText(text);
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event ->timerId() == id1){

        if(! P_Com->m_isConnect)       //结构体   对象->变量
        {
            //没有连接
            return ;
        }
       P_Com->ReadMyCom();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::MainWindow_Quit()
{
    this->close();
}

void MainWindow::MainWindow_About()
{
    if(P_About != NULL)
    {
        P_About->exec();
    }
}

void MainWindow::MainWindow_Communication()
{
 //   P_Com->search_usart();//查找可用串口

    if(P_Com != NULL)
    {
        P_Com->exec();
    }
}

//断开连接槽函数
void MainWindow::MainWindow_DisCommunication()
{
    if(P_Com != NULL)
    {
        P_Com->WorkClose();
    }
}
//勾选十六进制接收 注意这是在主界面
void MainWindow::on_checkBox_toggled(bool checked)
{
    P_Com->m_isHexDisplay = checked;
}

void MainWindow::on_ClearReceiveBuffer_clicked()
{
   ui->ReceiveDatatextBrowser->setText(tr(""));
}

void MainWindow::on_SendPushButton_clicked()
{
      P_Com->WorkSendMessage();
}
//勾选十六进制发送
void MainWindow::on_checkBox_2_toggled(bool checked)
{
     P_Com->m_isHexSend = checked;
}

void MainWindow::on_ReceiveStop_clicked()
{
    if(DisplayFlag)
    {
        ui->ReceiveStop->setText(tr("继续"));
        DisplayFlag = false;
    }
    else
    {
        ui->ReceiveStop->setText(tr("停止"));
        DisplayFlag = true;
    }
    P_Com->WorkSetDisplayFlag(DisplayFlag);
}

//另存为函数
bool MainWindow::SaveAs()
{
    QString curFile;
    QString fileName =QFileDialog::getSaveFileName(this,tr("另存为"),curFile,tr("TXT文档(*.txt);;所有文件(*.*)"));
    if(fileName.isEmpty())
    {
        return false;
    }
    if(!(fileName.endsWith(".txt",Qt::CaseInsensitive)))
    {
        fileName += ".txt";
    }
    QFile file(fileName);
    if(!(file.open(QFile::WriteOnly|QFile::Text)))
    {
        QMessageBox::critical(this,tr("critical"),tr("无法写入数据！"),QMessageBox::Yes);
        return false;
    }
    else
    {
        QTextStream out(&file);
        out<<ui->ReceiveDatatextBrowser->toPlainText();//移入out、file
        return true;
    }
}
//保存窗口数据 然后用connect关联到菜单栏
void MainWindow::on_SavePushButton_clicked()
{
    SaveAs();
}

void MainWindow::SlectAutoSend()
{
    P_Com->WorkSendMessage();
}

//自动定时发送
void MainWindow::on_AutoSend_toggled(bool checked)
{
    if(checked)
    {
        int time = ui->AutoSendTime->text().toInt();
        myTime->start(time);
        ui->SendPushButton->setEnabled(false);//这个没有函数   这个就是使能发送按钮  然后在失能发送按钮就行了
    }
    else
    {
        myTime->stop();
        ui->SendPushButton->setEnabled(true);
    }
}

void MainWindow::on_ClearSendDataPushButton_clicked()
{
    ui->textEdit->setText(tr(""));
}

/*弹窗函数  不用在哪儿调用  直接这样写一下 就可以 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::warning(this,tr("Warning"),tr("确认退出系统？"),QMessageBox::Yes,QMessageBox::No);
    switch(ret)
    {
        case QMessageBox::Yes:
            event->accept();
            this->close();
        break;
    default:
        event->ignore();
        break;
    }
}







