#pragma once
#include "Camera.h"
#include "cameraGuide.h"
#include "cameraSim.h"

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

	Camera **m_camera;
	static DWORD WINAPI _OperGuide(LPVOID lpParameter);
	static DWORD WINAPI _ShowGuide(LPVOID lpParameter);
	static DWORD WINAPI _OperImaging(LPVOID lpParameter);
	static DWORD WINAPI _ShowImaging(LPVOID lpParameter);
public:
	afx_msg void OnBnClickedBtnGuide();
	afx_msg void OnBnClickedBtnImaging();
};
