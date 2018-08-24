#pragma once
#include "Camera.h"
#include "cameraGuide.h"
#include "cameraImaging.h"
#include "cameraSim.h"
#include "Record.h"
#include "com.h"
#include "targetTrack.h"
#include "afxwin.h"

// CTrackerDlg 对话框
class CTrackerDlg : public CDialogEx
{
#define WM_UPDATEDATA WM_USER+1
// 构造
public:
	CTrackerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRACKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM IParam);

	int timeTest = 0;
	targetTrack targetTracker;

	void log(CString s);
	static DWORD WINAPI _MyTimer(LPVOID lpParameter);
	DWORD timeS[3] = { 0 }, timeE[3] = { 0 };
	int period[3] = { 1000 };
	bool isStart = true;

	com *comInst = NULL;
	void state(byte *buf);
	static DWORD WINAPI _SerRecv(LPVOID lpParameter);
	static DWORD WINAPI _TrackSend(LPVOID lpParameter);

	Camera **m_camera;
	static DWORD WINAPI _OperGuide(LPVOID lpParameter);
	static DWORD WINAPI _ShowGuide(LPVOID lpParameter);
	static DWORD WINAPI _OperImaging(LPVOID lpParameter);
	static DWORD WINAPI _ShowImaging(LPVOID lpParameter);
	bool triggerGuide = false,triggerImaging = false,triggerTrack = false;
	Record *recordGuide = NULL,*recordImaging = NULL;
public:
	afx_msg void OnBnClickedBtnGuide();
	afx_msg void OnBnClickedBtnImaging();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnEnable();
	afx_msg void OnBnClickedBtnDisable();
	afx_msg void OnBnClickedBtnLock();
	afx_msg void OnBnClickedBtnExcu();
	CListBox m_listLog;
	afx_msg void OnBnClickedBtnTrac();
	afx_msg void OnClose();
};
