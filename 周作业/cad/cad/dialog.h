#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QColorDialog> 

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public slots:
void slot1_color();
void slot_slidermove();
void slot1_StateChaget(int state);
private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
