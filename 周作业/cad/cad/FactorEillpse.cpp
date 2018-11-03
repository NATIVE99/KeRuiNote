#include "FactorEillpse.h"



CFactorEillpse::CFactorEillpse()
{
}


CFactorEillpse::~CFactorEillpse()
{
}

CMyChape* CFactorEillpse::CreateObject()
{
    return new CMyEllipse();

}