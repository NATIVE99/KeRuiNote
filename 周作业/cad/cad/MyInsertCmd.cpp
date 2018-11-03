#include "MyInsertCmd.h"



CMyInsertCmd::CMyInsertCmd(std::shared_ptr<CMyChape> shape, QVector<std::shared_ptr< CMyChape>>* list)
{
    m_shape = shape;
    m_list = list;
}


CMyInsertCmd::~CMyInsertCmd()
{
}


//执行方法
 void CMyInsertCmd::execute()
 {
     m_list->push_back(m_shape);
   
 }
//撤销方法
 void CMyInsertCmd::undo()
{
    
     m_list->removeOne(m_shape);

}