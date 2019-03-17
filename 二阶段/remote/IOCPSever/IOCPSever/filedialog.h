#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>
#include <QMenu>
#include "../../commd/commend.h"
#include <QFile>
namespace Ui {
class FileDialog;
}

class FileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDialog(SOCKET s,QWidget *parent = 0);
    ~FileDialog();
public slots:
    void slots_ShowMenu(const QPoint);
    void slots_Copy(bool b);
    void handleResults(QByteArray arr);
    void slots_Next(int Row, int col);
    void slots_HandleCopyFile(QByteArray arr);
private:
    Ui::FileDialog *ui;
    QMenu* m_PMenu;
    SOCKET m_client;
    QString Currpath;
    QString DownFileName;
};

#endif // FILEDIALOG_H
