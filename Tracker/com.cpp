#include "stdafx.h"
#include "com.h"

com::com()
{
}

com::com(LPCSTR port)
{
	hCom = CreateFileA(port,//串口名称
		GENERIC_READ | GENERIC_WRITE,//允许读写
		0, //独占方式
		NULL,
		OPEN_EXISTING,//打开而不是创建
		0,
		NULL);
	if (hCom == (HANDLE)-1)
	{
		hCom == NULL;
		return;
	}
	SetupComm(hCom, 100, 100);//输入输出缓冲区
	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = 57600;//波特率为9600bps  
	dcb.ByteSize = 8;//数据位8位  
	dcb.Parity = NOPARITY;//无校验  
	dcb.StopBits = ONESTOPBIT; //1停止位
	SetCommState(hCom, &dcb);
	isWork = true;

}

com::~com()
{
}

CString com::sendBuf(byte *buf,int cnt)
{
	OVERLAPPED m_osWrite;
	DWORD dwBytesWrite;
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	memset(&m_osWrite, 0, sizeof(OVERLAPPED));
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	dwBytesWrite = cnt;
	ClearCommError(hCom, &dwErrorFlags, &ComStat);//清除错误
	if (!WriteFile(hCom, buf, dwBytesWrite, &dwBytesWrite, &m_osWrite))	//发送
	{
		return _T("发送失败");
	}
	return _T("Send:") + byte2string(buf,cnt);
}

bool com::close()
{
	isWork = false;
	isTrack = false;
	CloseHandle(hCom);
	hCom = NULL;
	return true;
}

CString com::byte2string(byte *buf, int cnt)
{
	CString s;
	for(int i=0;i<cnt;i++)
		s.Format(_T("%s %02x"), s, buf[i]);
	return s;
}