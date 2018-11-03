#pragma once
#include "MyCommend.h"
#include "MyChape.h"
#include <memory>
class CMyMoveCmd :
    public CMyCommend
{
public:
    CMyMoveCmd(std::shared_ptr<CMyChape> shape, QVector<std::shared_ptr< CMyChape>>* list);
    ~CMyMoveCmd();
    //执行方法
    void execute();
    //撤销方法
     void undo();
private:
    std::shared_ptr<CMyChape> m_shape;
    QVector<std::shared_ptr< CMyChape>>* m_list;
};

