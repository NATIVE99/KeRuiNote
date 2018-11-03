#pragma once
#include "ToalFactor.h"
#include "CmyLine.h"
class CFactorLine:public CToalFactor
{
public:
    CFactorLine();
    ~CFactorLine();
public:
  virtual  CMyChape* CreateObject();

};

