#ifndef SPYXX_H
#define SPYXX_H

#include <QLabel>

class spyxx:public QLabel
{
    Q_OBJECT
public:
    spyxx(QWidget *parent = 0);
    void	mouseMoveEvent(QMouseEvent * ev);
    void	mousePressEvent(QMouseEvent * ev);
    void	mouseReleaseEvent(QMouseEvent * ev);
    void    draw(HWND hWnd);
signals:
    void SendHwnd(int hwnd);
private:
   HWND oldhwnd;

};

#endif // SPYXX_H
