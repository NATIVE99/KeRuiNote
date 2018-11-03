#include "cad.h"
Ui::cadClass* g_pUi;
cad::cad(QWidget *parent)
    : QMainWindow(parent), m_ActionGroup(parent)
{
    ui.setupUi(this);

    m_ActionGroup.addAction(ui.line);
    m_ActionGroup.addAction(ui.rect);
    m_ActionGroup.addAction(ui.eillpse);
    m_ActionGroup.addAction(ui.select);

    //给主窗口设置一个m_Scorll
   m_Scorll= new QScrollArea(this);

    m_Scorll->resize(600,800);
    //创建一个新窗口，父亲是m_Scorll
    m_fw = new Cframwnd(m_Scorll);
    m_fw->resize(600, 800);
   //把m_Scorll设置到中心位置
    setCentralWidget(m_Scorll);
    //m_Scorll设置m_fw窗口
    m_Scorll->setWidget(m_fw);
   
    //关联控件
    QObject::connect(ui.line, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerLine(bool)));
    QObject::connect(ui.rect, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerRect(bool)));
    QObject::connect(ui.eillpse, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerEillpse(bool)));
    QObject::connect(ui.color, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerColor(bool)));
    QObject::connect(ui.select, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerSelect(bool)));
    QObject::connect(ui.undo, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerUndo(bool)));
    QObject::connect(ui.exect, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerExect(bool)));

    QObject::connect(ui.open, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerOpen(bool)));
    QObject::connect(ui.save, SIGNAL(triggered(bool)), m_fw, SLOT(slots_tirgerSave(bool)));
    g_pUi = &ui;

}

void cad::slots_click(bool)
{
    QString str = "ssss";
    QMessageBox mes;
    mes.setText(str);
    mes.exec();
    
}

