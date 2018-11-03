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

    //������������һ��m_Scorll
   m_Scorll= new QScrollArea(this);

    m_Scorll->resize(600,800);
    //����һ���´��ڣ�������m_Scorll
    m_fw = new Cframwnd(m_Scorll);
    m_fw->resize(600, 800);
   //��m_Scorll���õ�����λ��
    setCentralWidget(m_Scorll);
    //m_Scorll����m_fw����
    m_Scorll->setWidget(m_fw);
   
    //�����ؼ�
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

