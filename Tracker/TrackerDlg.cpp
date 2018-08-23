
// TrackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Tracker.h"
#include "TrackerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTrackerDlg *g_pTrakerDlg;
int imagingID = 1;
float oriePosi = 0,pitcPosi = 0;


CTrackerDlg::CTrackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pTrakerDlg = this;
}

void CTrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
}

BEGIN_MESSAGE_MAP(CTrackerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
	ON_BN_CLICKED(IDC_BTN_GUIDE, &CTrackerDlg::OnBnClickedBtnGuide)
	ON_BN_CLICKED(IDC_BTN_IMAGING, &CTrackerDlg::OnBnClickedBtnImaging)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CTrackerDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_ENABLE, &CTrackerDlg::OnBnClickedBtnEnable)
	ON_BN_CLICKED(IDC_BTN_DISABLE, &CTrackerDlg::OnBnClickedBtnDisable)
	ON_BN_CLICKED(IDC_BTN_LOCK, &CTrackerDlg::OnBnClickedBtnLock)
	ON_BN_CLICKED(IDC_BTN_EXCU, &CTrackerDlg::OnBnClickedBtnExcu)
	ON_BN_CLICKED(IDC_BTN_TRAC, &CTrackerDlg::OnBnClickedBtnTrac)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

LRESULT CTrackerDlg::OnUpdateData(WPARAM wParam, LPARAM IParam)
{
	UpdateData(FALSE);
	return 0;
}

BOOL CTrackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	CComboBox *cbb = (CComboBox*)(GetDlgItem(IDC_CMB_SER));
	cbb->SetCurSel(0);
	m_camera = new Camera*[3]{NULL};

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTrackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CTrackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI CTrackerDlg::_OperGuide(LPVOID lpParameter)
{
	while (g_pTrakerDlg->m_camera[0]->isWork)
	{
		if (g_pTrakerDlg->triggerGuide)
		{
			g_pTrakerDlg->triggerGuide = false;
			g_pTrakerDlg->m_camera[0]->getData();
			g_pTrakerDlg->m_camera[0]->isShow = true;
			g_pTrakerDlg->recordGuide->write(g_pTrakerDlg->m_camera[imagingID]->imageBuffer);
		}
	}
	g_pTrakerDlg->recordGuide->stop();
	return 1;
}

DWORD WINAPI CTrackerDlg::_ShowGuide(LPVOID lpParameter)
{
	while (g_pTrakerDlg->m_camera[0]->isWork)
	{
		if (g_pTrakerDlg->m_camera[0]->isShow)
		{
			g_pTrakerDlg->m_camera[0]->show();
			g_pTrakerDlg->SendMessage(WM_UPDATEDATA, 0, 0);
			g_pTrakerDlg->m_camera[0]->isShow = false;
		}
	}
	return 1;
}

DWORD WINAPI CTrackerDlg::_OperImaging(LPVOID lpParameter)
{
	int i = 0;
	g_pTrakerDlg->timeTest = 0;
	while (g_pTrakerDlg->m_camera[imagingID]->isWork)
	{
		if (g_pTrakerDlg->triggerImaging)
		{
			g_pTrakerDlg->triggerImaging = false;
			g_pTrakerDlg->m_camera[imagingID]->getData();
			g_pTrakerDlg->m_camera[imagingID]->isShow = true;
			g_pTrakerDlg->recordImaging->write(g_pTrakerDlg->m_camera[imagingID]->imageBuffer);
			//i++;
		}
	}
	g_pTrakerDlg->recordImaging->stop();
	CString s;
	s.Format(_T("Test:%d"), g_pTrakerDlg->timeTest);
	g_pTrakerDlg->log(s);
	return 1;
}

DWORD WINAPI CTrackerDlg::_ShowImaging(LPVOID lpParameter)
{
	while (g_pTrakerDlg->m_camera[imagingID]->isWork)
	{
		if (g_pTrakerDlg->m_camera[imagingID]->isShow)
		{
			g_pTrakerDlg->m_camera[imagingID]->show();
			g_pTrakerDlg->SendMessage(WM_UPDATEDATA, 0, 0);
			g_pTrakerDlg->m_camera[imagingID]->isShow = false;
		}
	}
	return 1;
}

void CTrackerDlg::OnBnClickedBtnGuide()
{
	if (m_camera[0] == NULL)
	{
		CWnd *wnd = this->GetDlgItem(IDC_STATIC_GUIDE);
		m_camera[0] = new cameraGuide(wnd->GetDC());
		//SetTimer(1, 700 / g_pTrakerDlg->m_camera[0]->fps, 0);
		SetTimer(2, 20, 0);
		recordGuide = new Record(m_camera[0]->type, m_camera[0]->width, m_camera[0]->height, m_camera[0]->fps);
	}
	CString text;
	GetDlgItemText(IDC_BTN_GUIDE, text);
	if (text == "引导-启动")
	{
		if (m_camera[0]->open())
		{
			m_camera[0]->isWork = true;
			CreateThread(NULL, 0, _OperGuide, NULL, 0, NULL);
			CreateThread(NULL, 0, _ShowGuide, NULL, 0, NULL);
			SetDlgItemText(IDC_BTN_GUIDE, _T("引导-断开"));
			log(_T("引导相机启动"));
		}
		else
			log(_T("引导相机启动失败"));
	}
	else
	{
		m_camera[0]->isWork = false;
		m_camera[0]->close();
		SetDlgItemText(IDC_BTN_GUIDE, _T("引导-启动"));
		log(_T("引导相机断开"));
	}
}

void CTrackerDlg::OnBnClickedBtnImaging()
{
	if (m_camera[imagingID] == NULL)
	{
		CWnd *wnd = this->GetDlgItem(IDC_STATIC_IMAGING);
		m_camera[imagingID] = new cameraSim(wnd->GetDC());
		//SetTimer(2, 700 / g_pTrakerDlg->m_camera[imagingID]->fps, 0);
		SetTimer(2, 20, 0);
		recordImaging = new Record(m_camera[imagingID]->type, m_camera[imagingID]->width, m_camera[imagingID]->height, m_camera[imagingID]->fps);
	}
	CString text;
	GetDlgItemText(IDC_BTN_IMAGING, text);
	if (text == "成像-启动")
	{
		if (m_camera[imagingID]->open())
		{
			m_camera[imagingID]->isWork = true;
			CreateThread(NULL, 0, _OperImaging, NULL, 0, NULL);
			CreateThread(NULL, 0, _ShowImaging, NULL, 0, NULL);
			SetDlgItemText(IDC_BTN_IMAGING, _T("成像-断开"));
			log(_T("成像相机启动"));
		}
		else
			log(_T("成像相机启动失败"));
	}
	else
	{
		m_camera[imagingID]->isWork = false;
		m_camera[imagingID]->close();
		SetDlgItemText(IDC_BTN_IMAGING, _T("成像-启动"));
		log(_T("成像相机断开"));
	}
}

void CTrackerDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
		triggerGuide = true;
		break;
	case 2:
		timeTest++;
		triggerImaging = true;
		break;
	case 3:
		triggerTrack = true;
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

DWORD WINAPI CTrackerDlg::_SerRecv(LPVOID lpParameter)
{
	byte bufRecv[6] = { 0 };
	DWORD dwBytesRead = 100;
	byte *buf = new byte[dwBytesRead];
	OVERLAPPED m_osRead;
	memset(&m_osRead, 0, sizeof(OVERLAPPED));
	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while (g_pTrakerDlg->comInst->isWork)
	{
		COMSTAT ComStat;
		DWORD dwErrorFlags;
		memset(buf, 0, dwBytesRead);
		bool bReadStat = false;
		ClearCommError(g_pTrakerDlg->comInst->hCom, &dwErrorFlags, &ComStat);//清除错误
		if ((DWORD)ComStat.cbInQue == 0)
			continue;
		///*此处采用for循环是解决接收到的字符数不正确的问题，不知为何dwBytesRead不正确的原因*/
		for (int i = 0; i<22;)
		{
			bReadStat = ReadFile(g_pTrakerDlg->comInst->hCom, buf + i, dwBytesRead, &dwBytesRead, &m_osRead);
			i += dwBytesRead;
		}
		if (!bReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
				WaitForSingleObject(m_osRead.hEvent, 2000);
			/*使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟
			当串口读操作进行完毕后，m_osRead的hEvent事件会变为有信号*/
		}
		PurgeComm(g_pTrakerDlg->comInst->hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//清空缓冲区
		if (buf[0] != 0x55 || buf[1] != 0xBB)
		{
			g_pTrakerDlg->log(_T("接收数据帧头错误"));
		}
		else
		{
			memcpy(bufRecv, buf + 4, 6);
			g_pTrakerDlg->state(bufRecv);
			//g_pTrakerDlg->log(_T("Recv:") + g_pTrakerDlg->comInst->byte2string(buf, 22));
		}
	}
	g_pTrakerDlg->log(_T("Recv线程退出"));
	return 1;
}

DWORD WINAPI CTrackerDlg::_TrackSend(LPVOID lpParameter)
{
	int ori = 0, pit = 0, i = 0, j = 0;
	byte buf[39] = {0};
	while (g_pTrakerDlg->comInst->isTrack)
	{
		if (g_pTrakerDlg->triggerTrack)// && g_pTrakerDlg->comInst->isTrack)
		{
			g_pTrakerDlg->triggerTrack = false;
			ori = round(oriePosi / 360 * 32767);
			pit = round(pitcPosi / 360 * 32767);
			buf[0] = 0x55; buf[1] = 0xAA; buf[2] = 0x01; buf[3] = 0x02; buf[38] = 0xF0;
			buf[4] = 0x06;
			buf[29] = (uchar)(ori / 256);
			buf[30] = (uchar)(ori % 256);
			buf[31] = (uchar)(pit / 256);
			buf[32] = (uchar)(pit % 256);
			buf[37] = buf[0];
			for (i = 1; i<37; i++)
				buf[37] ^= buf[i];
			g_pTrakerDlg->comInst->sendBuf(buf, 39);
			j++;
		}
	}
	CString s;
	s.Format(_T("%d"), j);
	g_pTrakerDlg->log(s);
	g_pTrakerDlg->log(_T("Track线程退出"));
	return 1;
}

void CTrackerDlg::OnBnClickedBtnOpen()
{
	CString text;
	GetDlgItemText(IDC_BTN_OPEN, text);
	if (text == _T("连接"))
	{
		GetDlgItemText(IDC_CMB_SER, text);
		USES_CONVERSION;
		comInst = new com((LPCSTR)T2A(text));
		if (comInst->isWork)
		{
			CreateThread(NULL, 0, _SerRecv, NULL, 0, NULL);
			SetTimer(3, 15,0);
			SetDlgItemText(IDC_BTN_OPEN, _T("断开"));
			GetDlgItem(IDC_BTN_ENABLE)->EnableWindow(1);
			GetDlgItem(IDC_BTN_DISABLE)->EnableWindow(1);
			GetDlgItem(IDC_BTN_LOCK)->EnableWindow(1);
			GetDlgItem(IDC_BTN_EXCU)->EnableWindow(1);
			GetDlgItem(IDC_BTN_TRAC)->EnableWindow(1);
			log(_T("打开") + text + _T("成功"));
		}
		else
		{
			log(_T("打开") + text + _T("失败"));
		}
	}
	else
	{
		comInst->close();
		SetDlgItemText(IDC_BTN_OPEN, _T("连接"));
		GetDlgItem(IDC_BTN_ENABLE)->EnableWindow(0);
		GetDlgItem(IDC_BTN_DISABLE)->EnableWindow(0);
		GetDlgItem(IDC_BTN_LOCK)->EnableWindow(0);
		GetDlgItem(IDC_BTN_EXCU)->EnableWindow(0);
		GetDlgItem(IDC_BTN_TRAC)->EnableWindow(0);
		log(_T("串口关闭"));
	}
}

void CTrackerDlg::OnBnClickedBtnEnable()
{
	uchar buf[39] = { 0 };
	buf[0] = 0x55; buf[1] = 0xAA; buf[2] = 0x01; buf[3] = 0x02; buf[38] = 0xF0;
	buf[4] = 0x01;
	buf[37] = buf[0];
	for (int i = 1; i<37; i++)
		buf[37] ^= buf[i];
	log(comInst->sendBuf(buf, 39));
}

void CTrackerDlg::OnBnClickedBtnDisable()
{
	uchar buf[39] = { 0 };
	buf[0] = 0x55; buf[1] = 0xAA; buf[2] = 0x01; buf[3] = 0x02; buf[38] = 0xF0;
	buf[4] = 0x00;
	buf[37] = buf[0];
	for (int i = 1; i<37; i++)
		buf[37] ^= buf[i];
	log(comInst->sendBuf(buf, 39));
}

void CTrackerDlg::OnBnClickedBtnLock()
{
	uchar buf[39] = { 0 };
	buf[0] = 0x55; buf[1] = 0xAA; buf[2] = 0x01; buf[3] = 0x02; buf[38] = 0xF0;
	buf[4] = 0x02;
	buf[37] = buf[0];
	for (int i = 1; i<37; i++)
		buf[37] ^= buf[i];
	log(comInst->sendBuf(buf, 39));
}

void CTrackerDlg::OnBnClickedBtnExcu()
{
	CString x,y;
	GetDlgItemText(IDC_MANUAL_ORIE, x);
	GetDlgItemText(IDC_MANUAL_PITC, y);
	float posiX = _ttof(x);
	float posiY = _ttof(y);
	int ori = round(posiX / 360 * 32767);
	int pit = round(posiY / 360 * 32767);
	uchar buf[39] = { 0 };
	buf[0] = 0x55; buf[1] = 0xAA; buf[2] = 0x01; buf[3] = 0x02; buf[38] = 0xF0;
	buf[4] = 0x06;
	buf[29] = (uchar)(ori / 256);
	buf[30] = (uchar)(ori % 256);
	buf[31] = (uchar)(pit / 256);
	buf[32] = (uchar)(pit % 256);
	buf[37] = buf[0];
	for (int i = 1; i<37; i++)
		buf[37] ^= buf[i];
	log(comInst->sendBuf(buf, 39));
}

void CTrackerDlg::OnBnClickedBtnTrac()
{
	CString text;
	GetDlgItemText(IDC_BTN_TRAC, text);
	if (text == _T("跟踪"))
	{
		log(_T("开始跟踪"));
		comInst->isTrack = true;
		CreateThread(NULL, 0, _TrackSend, NULL, 0, NULL);
		SetDlgItemText(IDC_BTN_TRAC, _T("停止"));
	}
	else
	{
		comInst->isTrack = false;
		SetDlgItemText(IDC_BTN_TRAC, _T("跟踪"));
		log(_T("停止跟踪"));
	}
}

void CTrackerDlg::state(byte *buf)
{
	byte orientation = 0, pitching = 0;
	orientation = buf[1] & 0x03;
	pitching = (buf[1] & 0x0C) >> 2;
	switch (orientation) {
	case 1:
		((CButton*)GetDlgItem(IDC_ORIE_LEFT))->SetCheck(true);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_ORIE_RIGHT))->SetCheck(true);
		break;
	default:
		((CButton*)GetDlgItem(IDC_ORIE_NORM))->SetCheck(true);
		break;
	}
	switch (pitching) {
	case 1:
		((CButton*)GetDlgItem(IDC_PITC_UP))->SetCheck(true);
		break;
	case 2:
		((CButton*)GetDlgItem(IDC_PITC_DOWN))->SetCheck(true);
		break;
	default:
		((CButton*)GetDlgItem(IDC_PITC_NORM))->SetCheck(true);
		break;
	}
	float ori = 0, pit = 0;
	ori = buf[2] * 256 + buf[3];
	pit = buf[4] * 256 + buf[5];
	if (ori <= 0)
		ori = 0;
	else if (ori >= 32767)
		ori = 360;
	else
		ori = ori * 360 / 32767;
	if (pit <= 0)
		pit = 0;
	else if (ori >= 32767)
		pit = 360;
	else
		pit = pit * 360 / 32767;
	CString oriS, pitS;
	oriS.Format(_T("%.6f"), ori);
	SetDlgItemText(IDC_EDIT_ORIE, oriS);
	pitS.Format(_T("%.6f"), pit);
	SetDlgItemText(IDC_EDIT_PITC, pitS);
}

void CTrackerDlg::log(CString s) 
{
	CString time = CTime::GetCurrentTime().Format("%H:%M:%S ");
	m_listLog.AddString(CTime::GetCurrentTime().Format("%H:%M:%S ") + s);
	m_listLog.SetCurSel(m_listLog.GetCount() - 1);
}

void CTrackerDlg::OnClose()
{
	if(comInst!=NULL)
		if(comInst->isWork)
			comInst->close();
	if(m_camera[0]!=NULL)
		if (m_camera[0]->isWork)
		{
			m_camera[0]->isWork = false;
			m_camera[0]->close();
		}
	if(m_camera[imagingID]!=NULL)
		if (m_camera[imagingID]->isWork)
		{
			m_camera[imagingID]->isWork = false;
			m_camera[imagingID]->close();
		}
	CDialogEx::OnClose();
}
