#include "spyxx.h"
#include <qdebug.h>
#include <windows.h>
#include <QMouseEvent>
spyxx::spyxx(QWidget *parent ):QLabel(parent)
{

}

void spyxx::mouseMoveEvent(QMouseEvent *ev)
{

    POINT pt;
    pt.x = ev->globalX();
    pt.y = ev->globalY();
    HWND hwnd=WindowFromPoint(pt);


    //画矩形
    if(hwnd==NULL)
    {
        return;
    }
    if(oldhwnd==NULL)
    {
        // draw(oldhwnd);
        oldhwnd = hwnd;
        draw(hwnd);
        emit  SendHwnd((int)hwnd);
    }
    else
    {
        if(oldhwnd!=hwnd)
        {
            draw(oldhwnd);
            qDebug()<<"oldhwnd"<<oldhwnd;
            oldhwnd = hwnd;
            draw(hwnd);
            qDebug()<<"hwnd"<<hwnd;

            emit  SendHwnd((int)hwnd);
        }
    }



}

void spyxx::mousePressEvent(QMouseEvent *ev)
{

    this->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/rec/drag2.ico")));
    setCursor(QPixmap(QString::fromUtf8(":/new/prefix1/rec/eye.cur")));
    oldhwnd=NULL;

}

void spyxx::mouseReleaseEvent(QMouseEvent *ev)
{
    this->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/rec/drag.ico")));
    setCursor(Qt::ArrowCursor);

    draw(oldhwnd);
}

void spyxx::draw(HWND hWnd)
{
    //获取整个桌面DC
    HWND hDesktop = GetDesktopWindow();
    //需要画矩形的DC
    HDC hDC = GetDC(hDesktop);
    RECT rc;
    GetWindowRect(hWnd, &rc);
    //创建笔
    HPEN  hPen = CreatePen(PS_SOLID,
                           5,
                           RGB(0, 0, 0));
    //设置为异或笔
    SetROP2(hDC, R2_NOTXORPEN);
    //为dc选择画笔
    SelectObject(hDC, hPen);

    SelectObject(hDC, GetStockObject(NULL_BRUSH));
    //画矩形
    Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    //释放DC
    ReleaseDC(hDesktop, hDC);


}


