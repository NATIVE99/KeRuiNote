#include "FactorLine.h"



CFactorLine::CFactorLine()
{
}


CFactorLine::~CFactorLine()
{
}


CMyChape* CFactorLine::CreateObject()
{

    return new CmyLine();
}