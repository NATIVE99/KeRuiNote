#ifndef SCREENDIALOG_H
#define SCREENDIALOG_H

#include <QDialog>
#include <QPainter>
namespace Ui {
class ScreenDialog;
}

class ScreenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScreenDialog(QWidget *parent = 0);
    ~ScreenDialog();
    void paintEvent(QPaintEvent *event);
public slots:

    void Handledata(QByteArray data);


private:
    bool m_bDraw;
    QImage m_Img;
    Ui::ScreenDialog *ui;
};

#endif // SCREENDIALOG_H
