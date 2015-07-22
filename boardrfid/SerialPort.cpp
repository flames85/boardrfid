//////////////////////////////////////////////////////////////////////////  
/// COPYRIGHT NOTICE  
/// Copyright (c) 2009, 华中科技大学tickTick Group  （版权声明）  
/// All rights reserved.  
///   
/// @file    SerialPort.cpp    
/// @brief   串口通信类的实现文件  
///  
/// 本文件为串口通信类的实现代码  
///  
/// @version 1.0     
/// @author  卢俊    
/// @E-mail：lujun.hust@gmail.com  
/// @date    2010/03/19  
///   
///  
///  修订说明：  
//////////////////////////////////////////////////////////////////////////  
 
#include "StdAfx.h"  
#include "SerialPort.h"  
#include <process.h>  
#include <iostream>  
#include <sstream>
#include <iomanip>

//! 打印日志的缓存
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
 
/** 线程退出标志 */   
bool CSerialPort::s_bExit = false;  
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:秒 */   
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
 
    /** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */   
    char szDCBparam[50];  
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);  
 
    /** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
 
    /** 进入临界段 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 是否有错误发生 */   
    BOOL bIsSuccess = TRUE;  
 
    /** 在此可以设置输入输出的缓冲区大小,如果不设置,则系统会设置默认值.  
     *  自己设置缓冲区大小时,要注意设置稍大一些,避免缓冲区溢出  
     */ 
    /*if (bIsSuccess )  
    {  
        bIsSuccess = SetupComm(m_hComm,10,10);  
    }*/ 
 
    /** 设置串口的超时时间,均设为0,表示不使用超时限制 */ 
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
        // 将ANSI字符串转换为UNICODE字符串  
        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, NULL, 0);  
        wchar_t *pwText = new wchar_t[dwNum] ;  
        if (!MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, pwText, dwNum))  
        {  
            bIsSuccess = TRUE;  
        }  
 
        /** 获取当前串口配置参数,并且构造串口DCB参数 */   
        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb) ;  
        /** 开启RTS flow控制 */   
        dcb.fRtsControl = RTS_CONTROL_ENABLE;   
 
        /** 释放内存空间 */   
        delete [] pwText;  
    }  
 
    if ( bIsSuccess )  
    {  
        /** 使用DCB参数配置串口状态 */   
        bIsSuccess = SetCommState(m_hComm, &dcb);  
    }  
          
    /**  清空串口缓冲区 */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** 离开临界段 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return bIsSuccess==TRUE;  
}  
 
bool CSerialPort::InitPort( UINT portNo ,const LPDCB& plDCB )  
{  
    /** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
      
    /** 进入临界段 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 配置串口参数 */   
    if (!SetCommState(m_hComm, plDCB))  
    {  
        return false;  
    }  
 
    /**  清空串口缓冲区 */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** 离开临界段 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
void CSerialPort::ClosePort()  
{  
    /** 如果有串口被打开，关闭它 */ 
    if( m_hComm != INVALID_HANDLE_VALUE )  
    {  
        CloseHandle( m_hComm );  
        m_hComm = INVALID_HANDLE_VALUE;  
    }  
}  
 
bool CSerialPort::openPort( UINT portNo )  
{  
    /** 进入临界段 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 把串口的编号转换为设备名 */   
    char szPort[50];  
    sprintf_s(szPort, "COM%d", portNo);  
 
    /** 打开指定的串口 */   
    m_hComm = CreateFileA(szPort,  /** 设备名,COM1,COM2等 */   
              GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */     
              0,                            /** 共享模式,0表示不共享 */   
              NULL,                         /** 安全性设置,一般使用NULL */   
              OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */   
              0,      
              0);      
 
    /** 如果打开失败，释放资源并返回 */   
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        LeaveCriticalSection(&m_csCommunicationSync);  
        return false;  
    }  
 
    /** 退出临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
bool CSerialPort::OpenListenThread()  
{  
    /** 检测线程是否已经开启了 */   
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** 线程已经开启 */   
        return false;  
    }  
 
    s_bExit = false;  
    /** 线程ID */   
    UINT threadId;  
    /** 开启串口数据监听线程 */   
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);  
    if (!m_hListenThread)  
    {  
        return false;  
    }  
    /** 设置线程的优先级,高于普通线程 */   
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
        /** 通知线程退出 */   
        s_bExit = true;  
 
        /** 等待线程退出 */   
        Sleep(10);  
 
        /** 置线程句柄无效 */   
        CloseHandle( m_hListenThread );  
        m_hListenThread = INVALID_HANDLE_VALUE;  
    }  
    return true;  
}  
 
UINT CSerialPort::GetBytesInCOM()  
{  
    DWORD dwError = 0;  /** 错误码 */   
    COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */   
    memset(&comstat, 0, sizeof(COMSTAT));  
 
    UINT BytesInQue = 0;  
    /** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */   
    if ( ClearCommError(m_hComm, &dwError, &comstat) )  
    {  
        BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */   
    }  
 
    return BytesInQue;  
}  
 
UINT WINAPI CSerialPort::ListenThread( void* pParam )  
{  
    /** 得到本类的指针 */   
    CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);  
 
    // 线程循环,轮询方式读取串口数据  
    
    //! rfid字符串
    string strRfid;
    strRfid.clear();

    while (!pSerialPort->s_bExit)   
    {  
        UINT BytesInQue = pSerialPort->GetBytesInCOM();  
        /** 如果串口输入缓冲区中无数据,则休息一会再查询 */   
        if ( BytesInQue == 0 )  
        {  
            Sleep(SLEEP_TIME_INTERVAL);  
            continue;  
        }  
 
        /** 读取输入缓冲区中的数据并输出显示 */ 
        char cRecved = 0x00;
        do 
        {  
            cRecved = 0x00;  
            if(pSerialPort->ReadChar(cRecved) == true)  
            {  
                //std::cout << cRecved ;
                if('\r' == cRecved) // 结尾
                {
                    bool bRright = false;

                    if( 0 == strncmp(strRfid.c_str(), RFID_HEAD1, strlen(RFID_HEAD1))) // 为蓝海特殊处理的
                    {
                        strRfid = strRfid.substr(strlen(RFID_HEAD1)); // 去掉头部
                        bRright = true;
                    }
                    else if( 0 == strncmp(strRfid.c_str(), RFID_HEAD2, strlen(RFID_HEAD2)) ) // 普通的数据
                    {
                        strRfid = strRfid.substr(strlen(RFID_HEAD2)); // 去掉头部

                        //! :FDXB=06000000 C0F90080000000 解析成 999000 000000006 
                        //! :FDXB=66FD6508 C0F90000000000 解析成 999000 140901734
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
                            //! 日志
                            wstring wstrRfid;
                            StringToWstring(wstrRfid, strRfid);
                            _stprintf(logBuf, _T("{same-back-rfid} = {%s}\n"), wstrRfid.c_str());
                            OutputDebugString(logBuf);
                        }
                        else
                        {
                            while(m_rfidList.size() >= m_bufferSize)
                            {
                                //! 日志
                                _stprintf(logBuf, _T("{current-rfid-size,max-rfid-size} = {%d,%d}\n"), m_rfidList.size(), m_bufferSize);
                                OutputDebugString(logBuf);

                                //! 删去最前面的
                                string frontRfid = m_rfidList.front(); // 删除前先保存进字符串
                                m_rfidList.pop_front();

                                //! 日志
                                wstring wstrRfid;
                                StringToWstring(wstrRfid, frontRfid);
                                _stprintf(logBuf, _T("{pop-front-rfid,current-rfid-size} = {%s,%d}\n"), wstrRfid.c_str(), m_rfidList.size());
                                OutputDebugString(logBuf);
                            }
                            m_rfidList.push_back(strRfid); // 写入list
                            //! 日志
                            wstring wstrRfid;
                            StringToWstring(wstrRfid, strRfid);
                            _stprintf(logBuf, _T("{push-back-rfid,current-rfid-size} = {%s,%d}\n"), wstrRfid.c_str(), m_rfidList.size());
                            OutputDebugString(logBuf);
                        }
                    }
                    strRfid.clear(); // 清除临时数据
                }
                else
                {
                    strRfid.push_back(cRecved); // 单个字符写入临时数据
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

    //! 日志
    wstring wstrRfid;
    StringToWstring(wstrRfid, strRfid);
    _stprintf(logBuf, _T("{take-front-rfid,current-rfid-size} = {%s,%d}\n"), wstrRfid.c_str(), m_rfidList.size());
    OutputDebugString(logBuf);

    return true;
}

long CSerialPort::SetBufferSize(long nSize)
{
    //! 日志
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
 
    /** 临界区保护 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 从缓冲区读取一个字节的数据 */   
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        /** 获取错误码,可以根据该错误码查出错误原因 */   
        DWORD dwError = GetLastError();  
 
        /** 清空串口缓冲区 */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** 离开临界区 */   
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
 
    /** 临界区保护 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 向缓冲区写入指定量的数据 */   
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)    
    {  
        DWORD dwError = GetLastError();  
        /** 清空串口缓冲区 */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** 离开临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  