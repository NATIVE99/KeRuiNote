#include "screendialog.h"
#include "ui_screendialog.h"
ScreenDialog::ScreenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScreenDialog)
{
    ui->setupUi(this);
    m_bDraw = false;
}

ScreenDialog::~ScreenDialog()
{
    delete ui;
}

void ScreenDialog::Handledata(QByteArray data)
{
    //ÏÔÊ¾Í¼Æ¬Êý¾Ý
    char* pBuf = data.data();
    int nWidth = *(int*)pBuf;
    int nHeight = *((int*)pBuf + 1);


    QImage img(nWidth, nHeight, QImage::Format_ARGB32);
    memcpy(img.bits(), pBuf + 8, 4 * nWidth *nHeight);

    m_Img = img;

    m_bDraw = true;
    update();


}

void ScreenDialog::paintEvent(QPaintEvent *event)
{
    if (m_bDraw) {
        QPainter painter(this);

        QImage  img = m_Img.scaled(width(), height());

        painter.drawImage(0, 0, img);
    }

}
