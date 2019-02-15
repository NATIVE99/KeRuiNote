#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "enumpid.h"
#include "enumfile.h"
#include "enumserive.h"
#include "enumwin.h"
#include "enumreg.h"
#include "spyxx.h"
#include <QMenu>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
public slots:
    void showMenu(const QPoint &pos);
    void showServiceMenu(const QPoint &pos);
    void slots_RecvHwnd(int hwnd);
signals:

protected:
    void InitTablewidget();
    void InitFileEnum();
    void InitService();
private:
    Ui::MainWindow *ui;
    QMenu* pProcessMenu;
    QMenu* pServiceMenu;
    QAction* pStart;
    QAction* pEnd;

};

#endif // MAINWINDOW_H
