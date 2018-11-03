#pragma once
#include "MyCommend.h"
#include "MyChape.h"
#include <memory>
class CMyInsertCmd :
    public CMyCommend
{
public:
    CMyInsertCmd(std::shared_ptr<CMyChape> shape, QVector<std::shared_ptr< CMyChape>>* list);
    ~CMyInsertCmd();

    //执行方法
    virtual void execute();
    //撤销方法
    virtual void undo() ;
private:
    std::shared_ptr<CMyChape> m_shape;
    QVector<std::shared_ptr< CMyChape>>* m_list;
};

