#pragma once
class CMyCommend
{
public:
    CMyCommend();
    ~CMyCommend();

    //执行方法
    virtual void execute() = 0;
    //撤销方法
    virtual void undo() = 0;
};

