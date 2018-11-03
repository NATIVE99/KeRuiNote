#pragma once

#include <QWidget>
#include <memory>
#include <QMessageBox>
#include <qpainter.h>
#include <qdebug.h>
#include <QMouseEvent>
#include <memory>
#include <QDebug> 
#include <QPen>
#include <Qstack>
#include <QFileDialog>
#include "ui_cad.h"
#include "ToalFactor.h"
#include "FactorLine.h"
#include "FactorRect.h"
#include "FactorEillpse.h"
#include "dialog.h"
#include "MyCommend.h"
#include "MyInsertCmd.h"
#include "MyMoveCmd.h"
class Cframwnd:public QWidget
{
    Q_OBJECT
public:
     Cframwnd(QWidget *parent = Q_NULLPTR);
    ~Cframwnd();
public:
    virtual void paintEvent(QPaintEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);

    virtual  void mouseMoveEvent(QMouseEvent *event);

    virtual  void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void slots_tirgerLine(bool checked);
    void slots_tirgerRect(bool checked);
    void slots_tirgerEillpse(bool checked);
    void slots_tirgerColor(bool checked);
    void slots_tirgerSelect(bool checked);
    void slots_tirgerUndo(bool checked);
    void slots_tirgerExect(bool checked);
    void slots_tirgerOpen(bool checked);
    void slots_tirgerSave(bool checked);
private:
    QVector<std::shared_ptr< CMyChape>> m_lst;
    QPoint m_PointEnd;
    QPoint m_PointBegin;
    bool IsDraw;
    //int nSlect;
    CMyChape* m_Curshape;
    std::shared_ptr< CToalFactor> m_factor;
    
    std::shared_ptr< CMyChape> MoveShape;
    std::shared_ptr<CMyCommend> Curcmd;
    //³·ÏúÕ»
    QStack<std::shared_ptr<CMyCommend>> stack_undo;
    //Ö´ÐÐÕ»
    QStack<std::shared_ptr<CMyCommend>> stack_exe;
};

