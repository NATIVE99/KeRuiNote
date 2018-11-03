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
    //ִ�з���
    void execute();
    //��������
     void undo();
private:
    std::shared_ptr<CMyChape> m_shape;
    QVector<std::shared_ptr< CMyChape>>* m_list;
};

