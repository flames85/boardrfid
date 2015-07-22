//////////////////////////////////////////////////////////////////////////  
/// COPYRIGHT NOTICE  
/// Copyright (c) 2009, ���пƼ���ѧtickTick Group  ����Ȩ������  
/// All rights reserved.  
///   
/// @file    SerialPort.cpp    
/// @brief   ����ͨ�����ʵ���ļ�  
///  
/// ���ļ�Ϊ����ͨ�����ʵ�ִ���  
///  
/// @version 1.0     
/// @author  ¬��    
/// @E-mail��lujun.hust@gmail.com  
/// @date    2010/03/19  
///   
///  
///  �޶�˵����  
//////////////////////////////////////////////////////////////////////////  
 
#include "StdAfx.h"  
#include "SerialPort.h"  
#include <process.h>  
#include <iostream>  
#include <sstream>
#include <iomanip>

//! ��ӡ��־�Ļ���
TCHAR logBuf[512] = {0};
list<string> CSerialPort::m_rfidList;
#define RFID_HEAD1 ":FDXB(S)="
#define RFID_HEAD2 ":FDXB="

int str2int(const string &str, int nScale) 
{ 
    std::stringstream ss(str); 
    int num; 

    switch(nScale)
    {
    case 16:
        if((ss >> hex >> num).fail()) 
        {       
            // ERROR
        } 
        break;
    case 10:
        if((ss >> dec >> num).fail()) 
        {       
            //ERROR   
        } 
        break;
    default:
        // ERROR
        break;
    }

    return num;
}

const string int2str(int num, int nScale, int nWidth)
{
    stringstream strStream;  
    switch(nScale)
    {
    case 16:
        strStream << hex << setw(nWidth) << setfill('0') << num;  
        break;
    case 10:
        strStream << dec << setw(nWidth) << setfill('0') << num;  
        break;
    default:
        // ERROR
        break;
    }
    
    return strStream.str();  
}

long CSerialPort::m_bufferSize = 1;
void StringToWstring(std::wstring& szDst, std::string str)
{
    std::string temp = str;
    int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL,0); 
    wchar_t * wszUtf8 = new wchar_t[len+1]; 
    memset(wszUtf8, 0, len * 2 + 2); 
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
    szDst = wszUtf8;
    std::wstring r = wszUtf8;
    delete[] wszUtf8;
}
 
/** �߳��˳���־ */   
bool CSerialPort::s_bExit = false;  
/** ������������ʱ,sleep���´β�ѯ�����ʱ��,��λ:�� */   
const UINT SLEEP_TIME_INTERVAL = 5;  
 
CSerialPort::CSerialPort(void)
: m_hListenThread(INVALID_HANDLE_VALUE)
{  
    m_hComm = INVALID_HANDLE_VALUE;  
    m_hListenThread = INVALID_HANDLE_VALUE;  
 
    InitializeCriticalSection(&m_csCommunicationSync);  
 
}  
 
CSerialPort::~CSerialPort(void)  
{  
    CloseListenTread();  
    ClosePort();  
    DeleteCriticalSection(&m_csCommunicationSync);  
}  
 
bool CSerialPort::InitPort( UINT portNo /*= 1*/,UINT baud /*= CBR_9600*/,char parity /*= 'N'*/,  
                            UINT databits /*= 8*/, UINT stopsbits /*= 1*/,DWORD dwCommEvents /*= EV_RXCHAR*/ )  
{  
 
    /** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */   
    char szDCBparam[50];  
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);  
 
    /** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
 
    /** �����ٽ�� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �Ƿ��д����� */   
    BOOL bIsSuccess = TRUE;  
 
    /** �ڴ˿���������������Ļ�������С,���������,��ϵͳ������Ĭ��ֵ.  
     *  �Լ����û�������Сʱ,Ҫע�������Դ�һЩ,���⻺�������  
     */ 
    /*if (bIsSuccess )  
    {  
        bIsSuccess = SetupComm(m_hComm,10,10);  
    }*/ 
 
    /** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */ 
    COMMTIMEOUTS  CommTimeouts;  
    CommTimeouts.ReadIntervalTimeout         = 0;  
    CommTimeouts.ReadTotalTimeoutMultiplier  = 0;  
    CommTimeouts.ReadTotalTimeoutConstant    = 0;  
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;  
    CommTimeouts.WriteTotalTimeoutConstant   = 0;   
    if ( bIsSuccess)  
    {  
        bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);  
    }  
 
    DCB  dcb;  
    if ( bIsSuccess )  
    {  
        // ��ANSI�ַ���ת��ΪUNICODE�ַ���  
        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, NULL, 0);  
        wchar_t *pwText = new wchar_t[dwNum] ;  
        if (!MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, pwText, dwNum))  
        {  
            bIsSuccess = TRUE;  
        }  
 
        /** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */   
        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb) ;  
        /** ����RTS flow���� */   
        dcb.fRtsControl = RTS_CONTROL_ENABLE;   
 
        /** �ͷ��ڴ�ռ� */   
        delete [] pwText;  
    }  
 
    if ( bIsSuccess )  
    {  
        /** ʹ��DCB�������ô���״̬ */   
        bIsSuccess = SetCommState(m_hComm, &dcb);  
    }  
          
    /**  ��մ��ڻ����� */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** �뿪�ٽ�� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return bIsSuccess==TRUE;  
}  
 
bool CSerialPort::InitPort( UINT portNo ,const LPDCB& plDCB )  
{  
    /** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
      
    /** �����ٽ�� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** ���ô��ڲ��� */   
    if (!SetCommState(m_hComm, plDCB))  
    {  
        return false;  
    }  
 
    /**  ��մ��ڻ����� */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** �뿪�ٽ�� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
void CSerialPort::ClosePort()  
{  
    /** ����д��ڱ��򿪣��ر��� */ 
    if( m_hComm != INVALID_HANDLE_VALUE )  
    {  
        CloseHandle( m_hComm );  
        m_hComm = INVALID_HANDLE_VALUE;  
    }  
}  
 
bool CSerialPort::openPort( UINT portNo )  
{  
    /** �����ٽ�� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �Ѵ��ڵı��ת��Ϊ�豸�� */   
    char szPort[50];  
    sprintf_s(szPort, "COM%d", portNo);  
 
    /** ��ָ���Ĵ��� */   
    m_hComm = CreateFileA(szPort,  /** �豸��,COM1,COM2�� */   
              GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */     
              0,                            /** ����ģʽ,0��ʾ������ */   
              NULL,                         /** ��ȫ������,һ��ʹ��NULL */   
              OPEN_EXISTING,                /** �ò�����ʾ�豸�������,���򴴽�ʧ�� */   
              0,      
              0);      
 
    /** �����ʧ�ܣ��ͷ���Դ������ */   
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        LeaveCriticalSection(&m_csCommunicationSync);  
        return false;  
    }  
 
    /** �˳��ٽ��� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
bool CSerialPort::OpenListenThread()  
{  
    /** ����߳��Ƿ��Ѿ������� */   
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** �߳��Ѿ����� */   
        return false;  
    }  
 
    s_bExit = false;  
    /** �߳�ID */   
    UINT threadId;  
    /** �����������ݼ����߳� */   
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);  
    if (!m_hListenThread)  
    {  
        return false;  
    }  
    /** �����̵߳����ȼ�,������ͨ�߳� */   
    if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))  
    {  
        return false;  
    }  
 
    return true;  
}  
 
bool CSerialPort::CloseListenTread()  
{     
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** ֪ͨ�߳��˳� */   
        s_bExit = true;  
 
        /** �ȴ��߳��˳� */   
        Sleep(10);  
 
        /** ���߳̾����Ч */   
        CloseHandle( m_hListenThread );  
        m_hListenThread = INVALID_HANDLE_VALUE;  
    }  
    return true;  
}  
 
UINT CSerialPort::GetBytesInCOM()  
{  
    DWORD dwError = 0;  /** ������ */   
    COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */   
    memset(&comstat, 0, sizeof(COMSTAT));  
 
    UINT BytesInQue = 0;  
    /** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */   
    if ( ClearCommError(m_hComm, &dwError, &comstat) )  
    {  
        BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */   
    }  
 
    return BytesInQue;  
}  
 
UINT WINAPI CSerialPort::ListenThread( void* pParam )  
{  
    /** �õ������ָ�� */   
    CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);  
 
    // �߳�ѭ��,��ѯ��ʽ��ȡ��������  
    
    //! rfid�ַ���
    string strRfid;
    strRfid.clear();

    while (!pSerialPort->s_bExit)   
    {  
        UINT BytesInQue = pSerialPort->GetBytesInCOM();  
        /** ����������뻺������������,����Ϣһ���ٲ�ѯ */   
        if ( BytesInQue == 0 )  
        {  
            Sleep(SLEEP_TIME_INTERVAL);  
            continue;  
        }  
 
        /** ��ȡ���뻺�����е����ݲ������ʾ */ 
        char cRecved = 0x00;
        do 
        {  
            cRecved = 0x00;  
            if(pSerialPort->ReadChar(cRecved) == true)  
            {  
                //std::cout << cRecved ;
                if('\r' == cRecved) // ��β
                {
                    bool bRright = false;

                    if( 0 == strncmp(strRfid.c_str(), RFID_HEAD1, strlen(RFID_HEAD1))) // Ϊ�������⴦���
                    {
                        strRfid = strRfid.substr(strlen(RFID_HEAD1)); // ȥ��ͷ��
                        bRright = true;
                    }
                    else if( 0 == strncmp(strRfid.c_str(), RFID_HEAD2, strlen(RFID_HEAD2)) ) // ��ͨ������
                    {
                        strRfid = strRfid.substr(strlen(RFID_HEAD2)); // ȥ��ͷ��

                        //! :FDXB=06000000 C0F90080000000 ������ 999000 000000006 
                        //! :FDXB=66FD6508 C0F90000000000 ������ 999000 140901734
                        if(strRfid.size() < 8)
                        {
                            bRright = false;
                        }
                        else
                        {
                            std::string strShortRfid = strRfid.substr(0, 8);

                            std::string strShortRealRfid;
                            std::string strTemp;

                            strTemp = strShortRfid.substr(6, 2);
                            strShortRealRfid += strTemp;

                            strTemp = strShortRfid.substr(4, 2);
                            strShortRealRfid += strTemp;

                            strTemp = strShortRfid.substr(2, 2);
                            strShortRealRfid += strTemp;

                            strTemp = strShortRfid.substr(0, 2);
                            strShortRealRfid += strTemp;

                            int nRfid = str2int(strShortRealRfid, 16);

                            strRfid = int2str(nRfid, 10, 9);

                            strRfid = "999000" + strRfid;

                            bRright = true;
                        }
                    }
                    else
                    {
                        bRright = false;
                    }

                    if(bRright)
                    {
                        if(!m_rfidList.empty() && strRfid == m_rfidList.back())
                        {
                            //! ��־
                            wstring wstrRfid;
                            StringToWstring(wstrRfid, strRfid);
                            _stprintf(logBuf, _T("{same-back-rfid} = {%s}\n"), wstrRfid.c_str());
                            OutputDebugString(logBuf);
                        }
                        else
                        {
                            while(m_rfidList.size() >= m_bufferSize)
                            {
                                //! ��־
                                _stprintf(logBuf, _T("{current-rfid-size,max-rfid-size} = {%d,%d}\n"), m_rfidList.size(), m_bufferSize);
                                OutputDebugString(logBuf);

                                //! ɾȥ��ǰ���
                                string frontRfid = m_rfidList.front(); // ɾ��ǰ�ȱ�����ַ���
                                m_rfidList.pop_front();

                                //! ��־
                                wstring wstrRfid;
                                StringToWstring(wstrRfid, frontRfid);
                                _stprintf(logBuf, _T("{pop-front-rfid,current-rfid-size} = {%s,%d}\n"), wstrRfid.c_str(), m_rfidList.size());
                                OutputDebugString(logBuf);
                            }
                            m_rfidList.push_back(strRfid); // д��list
                            //! ��־
                            wstring wstrRfid;
                            StringToWstring(wstrRfid, strRfid);
                            _stprintf(logBuf, _T("{push-back-rfid,current-rfid-size} = {%s,%d}\n"), wstrRfid.c_str(), m_rfidList.size());
                            OutputDebugString(logBuf);
                        }
                    }
                    strRfid.clear(); // �����ʱ����
                }
                else
                {
                    strRfid.push_back(cRecved); // �����ַ�д����ʱ����
                }
                continue;  
            }  
        }while(--BytesInQue);  
    }  
 
    return 0;  
}  

bool CSerialPort::TakeRfid(string &strRfid)
{
    if(m_rfidList.empty()) 
    {
        return false;
    }
    
    strRfid = m_rfidList.front();
    m_rfidList.pop_front();

    //! ��־
    wstring wstrRfid;
    StringToWstring(wstrRfid, strRfid);
    _stprintf(logBuf, _T("{take-front-rfid,current-rfid-size} = {%s,%d}\n"), wstrRfid.c_str(), m_rfidList.size());
    OutputDebugString(logBuf);

    return true;
}

long CSerialPort::SetBufferSize(long nSize)
{
    //! ��־
    _stprintf(logBuf, _T("{set-max-rfid-size} = {%d}\n"), nSize);
    OutputDebugString(logBuf);

    if(nSize < 1) 
    {
        return -1;
    }
    m_bufferSize = nSize;
    return 0;
}
 
bool CSerialPort::ReadChar( char &cRecved )  
{  
    BOOL  bResult     = TRUE;  
    DWORD BytesRead   = 0;  
    if(m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  
 
    /** �ٽ������� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �ӻ�������ȡһ���ֽڵ����� */   
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        /** ��ȡ������,���Ը��ݸô�����������ԭ�� */   
        DWORD dwError = GetLastError();  
 
        /** ��մ��ڻ����� */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** �뿪�ٽ��� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return (BytesRead == 1);  
 
}  
 
bool CSerialPort::WriteData( unsigned char* pData, unsigned int length )  
{  
    BOOL   bResult     = TRUE;  
    DWORD  BytesToSend = 0;  
    if(m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  
 
    /** �ٽ������� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �򻺳���д��ָ���������� */   
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)    
    {  
        DWORD dwError = GetLastError();  
        /** ��մ��ڻ����� */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** �뿪�ٽ��� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  