#pragma once
class com
{
public:
	com();
	com(LPCSTR port);
	~com();
	CString sendBuf(byte *buf,int cnt);
	bool close();
	bool isWork = false;
	bool isTrack = false;
	HANDLE hCom = NULL;
	CString byte2string(byte *buf,int cnt);
};

