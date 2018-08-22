
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


CTrackerDlg::CTrackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pTrakerDlg = this;
}

void CTrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTrackerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UPDATEDATA, OnUpdateData)
	ON_BN_CLICKED(IDC_BTN_GUIDE, &CTrackerDlg::OnBnClickedBtnGuide)
	ON_BN_CLICKED(IDC_BTN_IMAGING, &CTrackerDlg::OnBnClickedBtnImaging)
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
		Sleep(1000/g_pTrakerDlg->m_camera[0]->fps);
		g_pTrakerDlg->m_camera[0]->getData();
		g_pTrakerDlg->m_camera[0]->isShow = true;
	}
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
	while (g_pTrakerDlg->m_camera[imagingID]->isWork)
	{
		Sleep(1000 / g_pTrakerDlg->m_camera[imagingID]->fps);
		g_pTrakerDlg->m_camera[imagingID]->getData();
		//FILE *fp = fopen("1.raw", "wb+");
		//fwrite(g_pTrakerDlg->m_camera[imagingID]->imageBuffer, 1, g_pTrakerDlg->m_camera[imagingID]->height*g_pTrakerDlg->m_camera[imagingID]->width, fp);
		//fclose(fp);
		g_pTrakerDlg->m_camera[imagingID]->isShow = true;
	}
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
		}
	}
	else
	{
		m_camera[0]->isWork = false;
		m_camera[0]->close();
		SetDlgItemText(IDC_BTN_GUIDE, _T("引导-启动"));
	}
}

void CTrackerDlg::OnBnClickedBtnImaging()
{
	if (m_camera[imagingID] == NULL)
	{
		CWnd *wnd = this->GetDlgItem(IDC_STATIC_IMAGING);
		m_camera[imagingID] = new cameraSim(wnd->GetDC());
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
		}
	}
	else
	{
		m_camera[imagingID]->isWork = false;
		m_camera[imagingID]->close();
		SetDlgItemText(IDC_BTN_IMAGING, _T("成像-启动"));
	}
}