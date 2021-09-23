#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Forms/aboutdialog.h"
#include "Forms/comdialog.h"
#include <QtGui>
#include <QMessageBox>
#include "QObject"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);
    void timerUpdata();
    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    ComDialog    *P_Com;//串口配置窗口声明
    AboutDialog  *P_About;//关于窗口声明
    QTimer       *myTime;

    bool     DisplayFlag;  //停止显示
    bool     SaveAs();     //另存为
    int           id1;
private slots:
    void MainWindow_Quit();  //关闭主界面
    void MainWindow_About(); //打开关于界面
    void MainWindow_Communication();//打开串口配置界面
    void MainWindow_DisCommunication();//断开连接

    void on_checkBox_toggled(bool checked);
    void on_ClearReceiveBuffer_clicked();
    void on_SendPushButton_clicked();
    void on_checkBox_2_toggled(bool checked);
    void on_ReceiveStop_clicked();
    void on_SavePushButton_clicked();
    void SlectAutoSend();
    void on_AutoSend_toggled(bool checked);
    void on_ClearSendDataPushButton_clicked();

};

#endif // MAINWINDOW_H
