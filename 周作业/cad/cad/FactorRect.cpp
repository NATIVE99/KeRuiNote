#include "FactorRect.h"



CFactorRect::CFactorRect()
{
}


CFactorRect::~CFactorRect()
{
}

CMyChape* CFactorRect::CreateObject()
{
    return new CMyRect();
}