#include "Cframwnd.h"
#include <qpoint.h>
extern Ui::cadClass* g_pUi;
Cframwnd::Cframwnd(QWidget *parent):QWidget(parent)
{
    IsDraw = true;
    m_Curshape = NULL;
    MoveShape = NULL;
    m_factor.reset(new CFactorLine());
    qRegisterMetaType<CmyLine>("CmyLine");
    qRegisterMetaType<CMyRect>("CMyRect");
    qRegisterMetaType<CMyEllipse>("CMyEllipse");

}


Cframwnd::~Cframwnd()
{
}

////////////////////////////////////////////////////////////////
/*槽*/
void Cframwnd::slots_tirgerLine(bool checked)
{

    m_factor.reset(new CFactorLine());
  
}
void Cframwnd::slots_tirgerRect(bool checked)
{

    m_factor.reset(new CFactorRect());
  
}

void  Cframwnd::slots_tirgerEillpse(bool checked)
{

    m_factor.reset(new CFactorEillpse());
 
}
void Cframwnd::slots_tirgerColor(bool checked)
{
    Dialog Dig;
    Dig.exec();


}
void Cframwnd::slots_tirgerSelect(bool checked)
{
    IsDraw = false;
}

void Cframwnd::slots_tirgerUndo(bool checked)
{
    if(stack_undo.count()>0)
    {
        std::shared_ptr<CMyCommend> cmd = stack_undo.top();
        cmd->undo();
        stack_undo.pop();
        stack_exe.push(cmd);
        update();
    }
 
}
void Cframwnd::slots_tirgerExect(bool checked)
{
    if (stack_exe.count()>0)
    {
        std::shared_ptr<CMyCommend> cmd = stack_exe.top();
        cmd->execute();
        stack_exe.pop();
        stack_undo.push(cmd);
        update();
    }
 
}

void Cframwnd::slots_tirgerOpen(bool checked)
{
    QString fileName = QFileDialog::getOpenFileName();
    QFile rFile(fileName);

    if(rFile.open(QIODevice::ReadOnly))
    {
        QDataStream ds(&rFile);
        QString Type;
        QPoint pointBegin;
        QPoint pointEnd;
        while(!ds.atEnd())
        {
            ds >> Type;
            ds >> pointBegin;
            ds >> pointEnd;
            std::shared_ptr<CMyChape> shape;
            int id = QMetaType::type(Type.toStdString().c_str());
            if (id != QMetaType::UnknownType) {
                shape.reset((CMyChape*)QMetaType::create(id));

                shape->SetPointBegin(pointBegin);
                shape->SetPointEnd(pointEnd);
                m_lst.push_back(shape);
            }
        }
      
    
    }

}
void Cframwnd::slots_tirgerSave(bool checked)
{
    //创建一个QFileDialog，用来获取文件路径
    QString fileName = QFileDialog::getSaveFileName();
    //打开保存的文件
    QFile file(fileName);
    //与Qdatastream绑定
    QDataStream ds(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        QVector<std::shared_ptr< CMyChape>>::iterator it;

        for (it = m_lst.begin(); it != m_lst.end(); it++)
        {
            (*it)->WriteFile(ds);
        }
    }

}
//事件
/////////////////////////////////////////////////

//绘制
void Cframwnd::paintEvent(QPaintEvent *event)
{
    QPixmap pm(800, 600);
    pm.fill(Qt::white);
    QPainter painter(&pm);



 
        QVector<std::shared_ptr< CMyChape>>::iterator it;

        for (it = m_lst.begin(); it != m_lst.end(); it++)
        {
            (*it)->OnDrow(painter);
        }

        if (m_Curshape != NULL)
        {
            m_Curshape->OnDrow(painter);
        }

  
  
    
       QPainter p(this);

        p.drawPixmap(0, 0, 800, 600, pm);

  
}
//鼠标按下
void Cframwnd::mousePressEvent(QMouseEvent *event)
{
  
    m_PointBegin = event->pos();
    if(g_pUi->select->isChecked())
    {
        IsDraw = false;
    }
    else 
    {
        IsDraw = true;
    }
    if(IsDraw)
    {
       
        m_Curshape = m_factor->CreateObject();
        m_Curshape->SetPointBegin(m_PointBegin);
    }
    else 
    {
        QVector<std::shared_ptr< CMyChape>>::reverse_iterator it;
     
        for (it = m_lst.rbegin(); it != m_lst.rend(); it++)
        {
            if((*it)->Contains(event->pos()))
            {
                if(MoveShape!=NULL)
                {
                    MoveShape->SetSelect(false);
                }
              
                    MoveShape = *it;
                    MoveShape->SetSelect(true);
                    Curcmd.reset(new CMyMoveCmd(MoveShape, &m_lst));
                    Curcmd->execute();
                    stack_undo.push(Curcmd);
                    m_lst.removeOne(*it);
               
                break;
            }
        }
     
    }

   

}
//鼠标移动
void Cframwnd::mouseMoveEvent(QMouseEvent *event)
{

    m_PointEnd = event->pos();
    if (IsDraw)
    {           
            m_Curshape->SetPointEnd(m_PointEnd);

    }
   else 
    {

       if (MoveShape != nullptr)
       {
           QPoint ptDiff = m_PointEnd - m_PointBegin;
           MoveShape->SetPointBegin(MoveShape->GetPointBegin() + ptDiff);
           MoveShape->SetPointEnd(MoveShape->GetPointEnd() + ptDiff);

           m_PointBegin = m_PointEnd;
         /*  Curcmd.reset(new CMyMoveCmd(MoveShape, &m_lst));
           Curcmd->execute();
           stack_undo.push(Curcmd);*/
       }
    }
    update();
 

}
//鼠标释放
void Cframwnd::mouseReleaseEvent(QMouseEvent *event)
{
       
       
             m_PointEnd = event->pos();
             if(IsDraw)
             {
                 IsDraw = false;
                                  
                 m_Curshape->SetPointEnd(m_PointEnd);
                 std::shared_ptr< CMyChape> shap(m_Curshape);
            
               // m_lst.push_back(shap);
                 
            
                 Curcmd.reset(new CMyInsertCmd(shap, &m_lst));
                 Curcmd->execute();
                 stack_undo.push(Curcmd);
                 //stack_exe.clear();
                 if(MoveShape!=NULL)
                 {
                     MoveShape->SetSelect(false);
                     MoveShape = NULL;
                 }
                 m_Curshape = NULL;
                
             }
            
             update();

       
      
   

}