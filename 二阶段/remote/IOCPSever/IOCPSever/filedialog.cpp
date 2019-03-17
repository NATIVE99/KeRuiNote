#include "filedialog.h"
#include "ui_filedialog.h"
#include <QDataStream>
FileDialog::FileDialog(SOCKET s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
    m_client = s;
    
    ui->tableWidget->setColumnCount(1);
    QStringList str;
    str << "FileName" ;
    ui->tableWidget->setHorizontalHeaderLabels(str);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

 
    m_PMenu = new QMenu;
    QAction* pCopy = new QAction(QStringLiteral("download"));
    m_PMenu->addAction(pCopy);
    QObject::connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint)), this, SLOT(slots_ShowMenu(const QPoint)));
    QObject::connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(slots_Next(int,int)));
    QObject::connect(pCopy, SIGNAL(triggered(bool)), this, SLOT(slots_Copy(bool)));


}

FileDialog::~FileDialog()
{
    delete ui;
}

void FileDialog::slots_ShowMenu(const QPoint)
{
    m_PMenu->exec(QCursor::pos());

}

void FileDialog::slots_Copy(bool b)
{
    QTableWidgetItem* pItem = ui->tableWidget->currentItem();
    DownFileName= pItem->text();
    QString path= ui->lineEdit->text() +"\\"+pItem->text();
    QByteArray date = path.toLocal8Bit();
    SendData2(m_client, CUT_DOWNLOAD, date.data(), date.size() + 1);
}

void FileDialog::slots_HandleCopyFile(QByteArray arr)
{

    QFile file(DownFileName);
    if (!file.open(QIODevice::WriteOnly| QIODevice::Append))
        return;

    file.write(arr);
    file.close();

    //QDataStream out(&file);
    //out << arr;
}
struct tagData
{
    int nLength;
    char  data[];
};
void FileDialog::handleResults(QByteArray arr)
{
    ui->tableWidget->clear();
    if(arr.size()==516)
    {

        char* szbuf =arr.data()+4;
        int n = 0;
        int nRow = 0;
        do
        {
            szbuf[n + 2] = 0;
        ui->tableWidget->insertRow(nRow);
        QTableWidgetItem* pItem = new QTableWidgetItem(&szbuf[n]);
        ui->tableWidget->setItem(nRow, 0, pItem);
        n = n + 4;
        nRow++;
        } while (szbuf[n]);
    }
    else
    {
        int nRow = 0;
        while (!arr.isEmpty())
        {
            tagData* ptag = (tagData*)arr.data();
            int nLength = ptag->nLength;

            char* pDate = new char[nLength + 2];
            memset(pDate, 0, nLength + 2);
            memcpy(pDate, arr.data() + sizeof(tagData), nLength);
            QString str = QString::fromLocal8Bit(pDate);

            ui->tableWidget->insertRow(nRow);
            QTableWidgetItem* pItem = new QTableWidgetItem(str);
            ui->tableWidget->setItem(nRow, 0, pItem);
            arr.remove(0, nLength + sizeof(tagData));
            nRow++;
        }
    }
}

void FileDialog::slots_Next(int Row, int col)
{
    QTableWidgetItem* pItem = ui->tableWidget->item(Row, col);
    QString path = ui->lineEdit->text();
    QString comp = QStringLiteral("´ËµçÄÔ");
    if(path== comp)
    {
        Currpath = pItem->text();
    }
    else
    {
        Currpath = Currpath+"\\"+pItem->text();
    }
    
    ui->lineEdit->setText(Currpath);

    QString str = Currpath + "\\*";
    QByteArray date=str.toLocal8Bit();
    SendData2(m_client, CUT_FILE, date.data(), date.size()+1);
}