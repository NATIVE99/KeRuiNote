#pragma once
class CMyCommend
{
public:
    CMyCommend();
    ~CMyCommend();

    //ִ�з���
    virtual void execute() = 0;
    //��������
    virtual void undo() = 0;
};

