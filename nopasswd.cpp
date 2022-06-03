#include "nopasswd.h"
#include "ui_nopasswd.h"
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>
#include <QPainter>
// 背景边框颜色，四个参数分别是 红、绿、蓝、不透明度
#define BORDER_COLOR QColor(49, 49, 49, 245)
// 窗口的背景颜色
#define BACKGROUND_COLOR QColor(49, 49, 49, 245)
// 窗口圆角的半径，单位：像素
#define RADIUS 15

noPasswd::noPasswd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::noPasswd)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);         //设置窗口背景透明
    boundaryWidth=4;                                    //设置触发resize的宽度
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void noPasswd::paintEvent(QPaintEvent * ev)
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

bool noPasswd::nativeEvent(const QByteArray &eventType, void *message, long *result)
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

void noPasswd::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        clickPos=e->pos();
}
void noPasswd::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
        move(e->pos()+pos()-clickPos);
}

noPasswd::~noPasswd()
{
    delete ui;
}

void noPasswd::on_passwd_textChanged(const QString &arg1)
{
    ui->passwd->setEchoMode(QLineEdit::Password);
}


void noPasswd::on_passwd2_textChanged(const QString &arg1)
{
    ui->passwd2->setEchoMode(QLineEdit::Password);
}


void noPasswd::on_username_cursorPositionChanged(int arg1, int arg2)
{
    QSettings username("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", QSettings::NativeFormat);
    QVariant value_username = username.value("DefaultUserName");
    QString usr = value_username.toString();
    ui->username->setText(usr);
    //ui->username->setFocusPolicy(Qt::NoFocus);
    //ui->username->setEnabled(false);
    //ui->username->setReadOnly(true);
}


void noPasswd::on_enterBtn_clicked()
{
    QMessageBox message;
    message.setStyleSheet("QMessageBox{background-color:#151a1e;}QLabel{color:#d3dae3;}");
    QString done = "OK!";
    QString err = "两次密码不相同！";
    QString pwd = ui->passwd->text();
    QString pwd2 = ui->passwd2->text();
    QSettings setting("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", QSettings::NativeFormat);
    if(pwd==pwd2){
        setting.setValue("AutoAdminLogon", 1);
        setting.setValue("DefaultPassword", pwd);
        message.setText(done);
        message.exec();
    }else{
        message.setText(err);
        message.exec();
    }
}




void noPasswd::on_pushButton_5_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

