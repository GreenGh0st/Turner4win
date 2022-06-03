#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include "nopasswd.h"
#include <iostream>
#include <QSettings>
#include <qprocess.h>
#include <QMessageBox>
#include <QFile>
#include <QUrl>
#include <QDesktopServices>
#include <QGraphicsEffect>
#include <QPainter>
#define BORDER_COLOR QColor(49, 49, 49, 240)// 背景边框颜色，四个参数分别是 红、绿、蓝、不透明度
#define BACKGROUND_COLOR QColor(49, 49, 49, 240)// 窗口的背景颜色
#define RADIUS 15// 窗口圆角的半径，单位：像素

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Tuner4win");
    setWindowIcon(QIcon(":/icon/C:/Users/a9149/Desktop/icon.png"));
    setAttribute(Qt::WA_TranslucentBackground);         //设置窗口背景透明
    boundaryWidth=4;                                    //设置触发resize的宽度
    this->setWindowFlags(Qt::FramelessWindowHint);      //设置为无边框窗口
    //setWindowOpacity(0.95);                           //设置窗口透明度
    //QGraphicsBlurEffect *blureffect = new QGraphicsBlurEffect;//模糊效果
    //blureffect->setBlurRadius(30);	//数值越大，越模糊
    //this->setGraphicsEffect(blureffect);

    QSettings launchTo("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
    QVariant value_launchTo = launchTo.value("LaunchTo");
    if(value_launchTo == 1){
        ui->radioButton->setChecked(1);
    }else if(value_launchTo == 2){
        ui->radioButton_2->setChecked(1);
    }else if(value_launchTo == 3){
        ui->radioButton_3->setChecked(1);
    }

    QSettings showPC("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel", QSettings::NativeFormat);
    QVariant value_showPC = showPC.value("{20D04FE0-3AEA-1069-A2D8-08002B30309D}");
    if(value_showPC == 0){
        ui->checkBox_2->setCheckState(Qt::Checked);
    }

    QSettings showSecond("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
    QVariant value_showSecond = showSecond.value("ShowSecondsInSystemClock");
    if(value_showSecond == 1){
        ui->checkBox->setCheckState(Qt::Checked);
    }

    QSettings noAntiSpyware("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows Defender", QSettings::NativeFormat);
    QVariant value_noAntiSpyware = noAntiSpyware.value("DisableAntiSpyware");
    if(value_noAntiSpyware == 1){
        ui->checkBox_3->setCheckState(Qt::Checked);
    }

    QSettings noSmartScreen("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\MicrosoftEdge\\PhishingFilter", QSettings::NativeFormat);
    QVariant value_noSmartScreen = noSmartScreen.value("EnabledV9");
    if(value_noSmartScreen == 1){
        ui->checkBox_4->setCheckState(Qt::Checked);
    }

    QSettings numLockOn("HKEY_USERS\\.DEFAULT\\Control Panel\\Keyboard", QSettings::NativeFormat);
    QVariant value_numLockOn = numLockOn.value("InitialKeyboardIndicators");
    if(value_numLockOn == 2){
        ui->checkBox_5->setCheckState(Qt::Checked);
    }

    QSettings hideFileExt("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
    QVariant value_hideFileExt = hideFileExt.value("HideFileExt");
    if(value_hideFileExt == 0){
        ui->checkBox_6->setCheckState(Qt::Checked);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent * ev)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 设置抗锯齿

    QPen pen(BORDER_COLOR);
    pen.setWidth(1);  // 设置边框为 1 像素
    painter.setPen(pen);

    QBrush brush(BACKGROUND_COLOR);
    painter.setBrush(brush);

    QRectF rect(0, 0, this->width(), this->height());  // 画一个和窗口一样大的矩形
    painter.drawRoundedRect(rect, RADIUS, RADIUS);  // 设置矩形的圆角
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) //无边框窗口用鼠标移动窗口
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
        {
        case WM_NCHITTEST:
            int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
            int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
            if(xPos < boundaryWidth && yPos<boundaryWidth)                    //左上角
                *result = HTTOPLEFT;
            else if(xPos>=width()-boundaryWidth&&yPos<boundaryWidth)          //右上角
                *result = HTTOPRIGHT;
            else if(xPos<boundaryWidth&&yPos>=height()-boundaryWidth)         //左下角
                *result = HTBOTTOMLEFT;
            else if(xPos>=width()-boundaryWidth&&yPos>=height()-boundaryWidth)//右下角
                *result = HTBOTTOMRIGHT;
            else if(xPos < boundaryWidth)                                     //左边
                *result =  HTLEFT;
            else if(xPos>=width()-boundaryWidth)                              //右边
                *result = HTRIGHT;
            else if(yPos<boundaryWidth)                                       //上边
                *result = HTTOP;
            else if(yPos>=height()-boundaryWidth)                             //下边
                *result = HTBOTTOM;
            else              //其他部分不做处理，返回false，留给其他事件处理器处理
               return false;
            return true;
        }
        return false;         //此处返回false，留给其他事件处理器处理
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
        move(e->pos()+pos()-clickPos);
}

void MainWindow::on_radioButton_3_clicked()
{
    QSettings setting("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
    setting.setValue("LaunchTo", 3);

}


void MainWindow::on_radioButton_2_clicked()
{
    QSettings setting("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
    setting.setValue("LaunchTo", 2);

}


void MainWindow::on_radioButton_clicked()
{
    QSettings setting("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
    setting.setValue("LaunchTo", 1);

}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(ui->checkBox_2->checkState() == Qt::Checked)
    {
        QSettings setting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel", QSettings::NativeFormat);
        setting.setValue("{20D04FE0-3AEA-1069-A2D8-08002B30309D}", 0);
    }
    else if(ui->checkBox_2->checkState() == Qt::Unchecked)
    {
        QSettings setting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\HideDesktopIcons\\NewStartPanel", QSettings::NativeFormat);
        setting.setValue("{20D04FE0-3AEA-1069-A2D8-08002B30309D}", 1);
    }
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->checkState() == Qt::Checked)
    {
        QSettings setting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
        setting.setValue("ShowSecondsInSystemClock", 1);
    }
    else if(ui->checkBox_2->checkState() == Qt::Unchecked)
    {
        QSettings setting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
        setting.setValue("ShowSecondsInSystemClock", 0);
    }
}


void MainWindow::on_pushButton_clicked()
{
    QProcess p(0),t(0);
    QMessageBox testMassage;
    testMassage.setStyleSheet("QMessageBox{background-color:#151a1e;}QLabel{color:#d3dae3;}");
    p.start("cmd", QStringList()<<"/c"<<"taskkill /f /im explorer.exe & start explorer.exe");
    p.waitForStarted();
    p.waitForFinished();
    QString strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());

    testMassage.setText("已重启！");
    testMassage.exec();

}


void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(ui->checkBox_3->checkState() == Qt::Checked)
    {
        QSettings setting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows Defender", QSettings::NativeFormat);
        setting.setValue("DisableAntiSpyware", 1);
    }
    else if(ui->checkBox_3->checkState() == Qt::Unchecked)
    {
        QSettings setting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\Windows Defender", QSettings::NativeFormat);
        setting.setValue("DisableAntiSpyware", 0);
    }
}


void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if(ui->checkBox_4->checkState() == Qt::Checked)
    {
        QSettings setting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\MicrosoftEdge\\PhishingFilter", QSettings::NativeFormat);
        setting.setValue("EnabledV9", 0);
    }
    else if(ui->checkBox_4->checkState() == Qt::Unchecked)
    {
        QSettings setting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Policies\\Microsoft\\MicrosoftEdge\\PhishingFilter", QSettings::NativeFormat);
        setting.setValue("EnabledV9", 1);
    }
}


void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    if(ui->checkBox_5->checkState() == Qt::Checked)
    {
        QSettings setting("HKEY_USERS\\.DEFAULT\\Control Panel\\Keyboard", QSettings::NativeFormat);
        setting.setValue("InitialKeyboardIndicators", "2");
    }
    else if(ui->checkBox_5->checkState() == Qt::Unchecked)
    {
        QSettings setting("HKEY_USERS\\.DEFAULT\\Control Panel\\Keyboard", QSettings::NativeFormat);
        setting.setValue("InitialKeyboardIndicators", "0");
    }
}


void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    if(ui->checkBox_6->checkState() == Qt::Checked)
    {
        QSettings setting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
        setting.setValue("HideFileExt", 0);
    }
    else if(ui->checkBox_6->checkState() == Qt::Unchecked)
    {
        QSettings setting("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", QSettings::NativeFormat);
        setting.setValue("HideFileExt", 1);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    noPasswd *np = new noPasswd();
    np->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    QDesktopServices::openUrl(QUrl("Activation.cmd"));
}



void MainWindow::on_pushButton_4_clicked()
{
    close();
}


void MainWindow::on_pushButton_5_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

