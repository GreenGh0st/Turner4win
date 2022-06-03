#ifndef NOPASSWD_H
#define NOPASSWD_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <windows.h>        //注意头文件
#include <windowsx.h>

namespace Ui {
class noPasswd;
}

class noPasswd : public QWidget
{
    Q_OBJECT

public:
    explicit noPasswd(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    ~noPasswd();

private slots:

    void on_passwd_textChanged(const QString &arg1);

    void on_passwd2_textChanged(const QString &arg1);

    void on_username_cursorPositionChanged(int arg1, int arg2);

    void on_enterBtn_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::noPasswd *ui;
    int boundaryWidth;
    QPoint clickPos;
};

#endif // NOPASSWD_H
