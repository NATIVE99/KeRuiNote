#include "MyInsertCmd.h"



CMyInsertCmd::CMyInsertCmd(std::shared_ptr<CMyChape> shape, QVector<std::shared_ptr< CMyChape>>* list)
{
    m_shape = shape;
    m_list = list;
}


CMyInsertCmd::~CMyInsertCmd()
{
}


//ִ�з���
 void CMyInsertCmd::execute()
 {
     m_list->push_back(m_shape);
   
 }
//��������
 void CMyInsertCmd::undo()
{
    
     m_list->removeOne(m_shape);

}