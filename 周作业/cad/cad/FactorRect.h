#pragma once
#include "ToalFactor.h"
#include "MyRect.h"
class CFactorRect:public CToalFactor
{
public:
    CFactorRect();
    ~CFactorRect();
public:
   virtual CMyChape* CreateObject();
};

