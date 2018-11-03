#pragma once
#include "MyChape.h"
class CToalFactor
{
public:
    CToalFactor();
    ~CToalFactor();
public:
    virtual CMyChape* CreateObject() = 0;

};

